<?php
/**
 * Page to select Project Timesheet Type to add new task.
 */
function project_timesheet_admin_add_page() {
  $items = array();
  foreach (project_timesheet_types() as $project_timesheet_type_key => $project_timesheet_type) {
    $items[] = l(entity_label('project_timesheet_type', $project_timesheet_type), PROJECT_TIMESHEET_ADMIN_URI . '/add/' . $project_timesheet_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of Project Timesheet to create.')));
}

/**
 * Add new Project Timesheet page callback.
 */
function project_timesheet_admin_add($type) {
  $project_timesheet_type = project_timesheet_types($type);

  $project_timesheet = entity_create('project_timesheet', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('project_timesheet_type', $project_timesheet_type))));

  $output = drupal_get_form('project_timesheet_admin_add_form', $project_timesheet);

  return $output;
}

function project_timesheet_get_all_project_name_order(){
  $options = array();

  $results = db_select('project', 'p,')
  ->fields('p', array('pid','name'))
  ->condition('p.type', 'project', '=')
  ->orderBy('p.name','ASC')
  ->execute();

  foreach ($results as $result) {
    $options[$result->pid] = $result->name;
  }
  return $options;
}
function project_timesheet_get_all_pm_name(){
  $options_pid = array();
  $results = db_select('people', 'p,')
  ->fields('p', array('pid','first_name','last_name'))
  ->condition('p.type', 'people', '=')
  ->orderBy('p.first_name','ASC')
  ->execute();

  foreach ($results as $result) {
    $options_pid[$result->pid] = $result->first_name.' '.$result->last_name;
  }
  return $options_pid;
}

function check_required_ajax($form, $form_state){
  return $form['number_contract_mandays'];
}
function check_required_ajax_new_contract_reason($form, $form_state){
  return $form['new_contract_reason'];
}
function check_required_reason_added_ajax($form, $form_state){
  return $form['reason_added'];
}

/**
 * Project Timesheet Form.
 */
function project_timesheet_admin_add_form($form, &$form_state, $project_timesheet) {
  $form_state['project_timesheet'] = $project_timesheet;

  $form['week'] = array(
      '#type' => 'textfield',
      '#title' => t('Week Number'). ' :',
      '#default_value' => date('W'),
      '#required' => true,
  );
  $form['reestimated_mandays'] = array(
      '#type'   => 'radios',
      '#options' => array(1 => t('Yes'), 0 => t('No')),
      '#title'  => t('Did you really re-estimated your mandays to finish the project ?'),
      '#required' => true,
  );
  $form['consumed_mandays'] = array(
      '#type' => 'textfield',
      '#title' => t('How many mandays did you consumed this week ?'),
      '#required' => true,
  );
  $form['mandays_to_finish'] = array(
      '#type' => 'textfield',
      '#title' => t('How many mandays do you need to finish the project ?'),
      '#required' => true,
  );
  $form['staff_cost'] = array(
      '#type' => 'textfield',
      '#title' => t('How much is the staff cost this week ?'),
      '#required' => true,
  );

  $form['budget_to_finish'] = array(
      '#type' => 'textfield',
      '#title' => t('Budget to finish'),
      '#required' => true,
  );
  $form['added_mandays'] = array(
      '#type' => 'textfield',
      '#title' => t('Added mandays'),
      '#required' => true,
      '#ajax' => array(
          'progress' => array(
              'type' => 'throbber',
              'message' => ''
          ),
          'callback' => 'check_required_reason_added_ajax',
          'wrapper' => 'check_replace_field_reason_added',

      ),
  );
  $form['reason_added'] = array(
      '#type' => 'textarea',
      '#title' => t('Reason added'),
      '#required' =>(isset($form_state['values']['added_mandays']) ? ($form_state['values']['added_mandays'] <> 0 ? TRUE : FALSE) : FALSE),
      '#prefix' => '<div id="check_replace_field_reason_added">',
      '#suffix' => '</div>',
  );
  $form['new_contract_mandays'] = array(
      '#title' => t('New Contract Mandays'),
      '#type' => 'radios',
      '#options' => array(1 => t('Yes'), 0 => t('No')),
      '#default_value' => 0,
      '#ajax' => array(
          'progress' => array(
              'type' => 'throbber',
              'message' => ''
          ),
          'callback' => 'check_required_ajax',
          'wrapper' => 'check_replace_field_div',

      ),
  );
  $form['number_contract_mandays'] = array(
      '#type' => 'textfield',
      '#title' => t('Number Contract Mandays'),
      '#required' => (isset($form_state['values']['new_contract_mandays']) ? ($form_state['values']['new_contract_mandays'] == true ? TRUE : FALSE) : FALSE),
      /* '#states' => array(
          "visible" => array(
              "input[name='new_contract_mandays']" => array("checked" => TRUE)),
      ), */
      '#prefix' => '<div id="check_replace_field_div">',
      '#suffix' => '</div>',
      '#ajax' => array(
          'progress' => array(
              'type' => 'throbber',
              'message' => ''
          ),
          'callback' => 'check_required_ajax_new_contract_reason',
          'wrapper' => 'check_replace_field_new_contract_reason',
      ),
  );
  $form['new_contract_reason'] = array(
      '#type' => 'textfield',
      '#title' => t('New Contract Reason'),
      '#required' => (isset($form_state['values']['number_contract_mandays']) ? ($form_state['values']['number_contract_mandays'] <> '' ? TRUE : FALSE) : FALSE),
      '#prefix' => '<div id="check_replace_field_new_contract_reason">',
      '#suffix' => '</div>',
  );
  $form['mandays_delta'] = array(
      '#type' => 'hidden',
      '#title' => t('Mandays delta'),
  );
  $form['budget_delta'] = array(
      '#type' => 'hidden',
      '#title' => t('Budget delta'),
  );
  $form['project_status'] = array(
      '#type'   => 'radios',
      '#options' => array(1 => t('In Progress'), 2 => t('Paused'), 3 => t('Closed')),
      '#title'  => t('Project Status'),
      '#default_value' => 1,
  );
  $form['accounting_status'] = array(
      '#type'   => 'select',
      '#options' => array(1 => t('Nothing Special this week'), 2 => t('Liquidable'), 3 => t('Invoicable in full'), 4 => t('Partially invoicable')),
      '#title'  => t('Accounting Status'),
      '#default_value' => 1,
  );
  $form['comment'] = array(
      '#type' => 'textarea',
      '#title' => t('Comment'),
  );
  $form['timesheet_status'] = array(
      '#type'   => 'radios',
      '#options' => array(1 => t('Todo'), 2 => t('Submitted'), 3 => t('Approved')),
      '#title'  => t('Timesheet Status'),
  );
  $options_ptid = array();
  $options_ptid = project_timesheet_get_all_project_name_order();
  $form['project'] = array(
      '#title' => t('Project Name'),
      '#type' => 'select',
      '#options' => $options_ptid,
  );

  $options_pid = array();
  $options_pid = project_timesheet_get_all_pm_name();
  $form['pm_on_creation'] = array(
      '#title' => t('Project Manager'),
      '#type' => 'select',
      '#options' => $options_pid,
  );

  field_attach_form('project_timesheet', $project_timesheet, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('project_timesheet_admin_add_form_submit'),
  );

  // Show Delete button if we edit task.
  $project_timesheet_id = entity_id('project_timesheet' ,$project_timesheet);
  if (!empty($project_timesheet_id) && project_timesheet_access('edit', $project_timesheet)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('project_timesheet_admin_add_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'project_timesheet_admin_add_form_validate';

  return $form;
}

function project_timesheet_admin_add_form_validate($form, &$form_state) {

}

/**
 * Project Timesheet submit handler.
 */
function project_timesheet_admin_add_form_submit($form, &$form_state) {
  $project_timesheet = $form_state['project_timesheet'];

  $project_id = $form_state['values']['project'];
  $consumed_mandays = $form_state['values']['consumed_mandays'];
  $mandays_to_finish = $form_state['values']['mandays_to_finish'];
  $current_mandays = project_timesheet_get_current_mandays($project_id);
  $mandays_delta = round(($consumed_mandays + $mandays_to_finish - $current_mandays) / $current_mandays, 2) ;

  $staff_cost = $form_state['values']['staff_cost'];
  $budget_to_finish = $form_state['values']['budget_to_finish'];
  $current_budget = project_timesheet_get_current_budget($project_id);
  $budget_delta = round(($staff_cost + $budget_to_finish - $current_budget) / $current_budget, 2) ;

  $form_state['values']['mandays_delta'] = $mandays_delta;
  $form_state['values']['budget_delta'] = $budget_delta;


  entity_form_submit_build_entity('project_timesheet', $project_timesheet, $form, $form_state);

  project_timesheet_save($project_timesheet);

  $project_timesheet_uri = entity_uri('project_timesheet', $project_timesheet);

  $form_state['redirect'] = $project_timesheet_uri['path'];

  drupal_set_message(t('Project Timesheet %title saved.', array('%title' => entity_label('project_timesheet', $project_timesheet))));
}

function project_timesheet_get_current_mandays($project_id){
  return db_select('project', 'pt,')
  ->fields('pt', array('current_mandays'))
  ->condition('pt.pid', $project_id, '=')
  ->execute()
  ->fetchField();
}

function project_timesheet_get_current_budget($project_id){
  return db_select('project', 'pt,')
  ->fields('pt', array('current_budget'))
  ->condition('pt.pid', $project_id, '=')
  ->execute()
  ->fetchField();
}

function project_timesheet_admin_add_form_submit_delete($form, &$form_state) {
  $project_timesheet = $form_state['project_timesheet'];
  $project_timesheet_uri = entity_uri('project_timesheet', $project_timesheet);
  $form_state['redirect'] = $project_timesheet_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function project_timesheet_admin_delete_form($form, &$form_state, $project_timesheet) {
  $form_state['project_timesheet'] = $project_timesheet;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['project_timesheet_type_id'] = array('#type' => 'value', '#value' => entity_id('project_timesheet' ,$project_timesheet));
  $project_timesheet_uri = entity_uri('project_timesheet', $project_timesheet);
  return confirm_form($form,
      t('Are you sure you want to delete Project Timesheet %title?', array('%title' => entity_label('project_timesheet', $project_timesheet))),
      $project_timesheet_uri['path'],
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function project_timesheet_admin_delete_form_submit($form, &$form_state) {
  $project_timesheet = $form_state['project_timesheet'];
  project_timesheet_delete($project_timesheet);

  drupal_set_message(t('Project Timesheet %title deleted.', array('%title' => entity_label('project_timesheet', $project_timesheet))));

  $form_state['redirect'] = '<front>';
}