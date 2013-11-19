<?php
/**
 * Page to select Project Type to add new task.
 */
function project_admin_add_page() {
  $items = array();
  foreach (project_types() as $project_type_key => $project_type) {
    $items[] = l(entity_label('project_type', $project_type), PROJECT_ADMIN_URI . '/add/' . $project_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of Project to create.')));
}

/**
 * Add new Project page callback.
 */
function project_admin_add($type) {
  $project_type = project_types($type);

  $project = entity_create('project', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('project_type', $project_type))));

  $output = drupal_get_form('project_admin_add_form', $project);

  return $output;
}

function get_all_id_people(){
  $options_pid = array('0' => '- None -');
  $results = db_select('people', 'p')
  ->fields('p', array('pid','first_name','last_name'))
  ->condition('p.type', 'people', '=')
  ->orderBy('p.first_name', 'ASC')
  ->execute();
  foreach ($results as $result) {
    $options_pid[$result->pid] = $result->first_name.' '.$result->last_name;
  }
  return $options_pid;
}

function get_all_id_account(){
  $options_uid = array('0' => '- None -');
  $results = db_select('users', 'u')
  ->fields('u', array('uid','name'))
  ->condition('u.status', '1', '=')
  ->condition('u.uid', '0', '>')
  ->orderBy('u.name', 'ASC')
  ->execute();
  foreach ($results as $result) {
    $options_uid[$result->uid] = $result->name;
  }
  return $options_uid;
}
/**
 * Project Form.
 */
function project_admin_add_form($form, &$form_state, $project) {
  $form_state['project'] = $project;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Project Name'),
    '#required' => true,
  );
  $form['code'] = array(
      '#type' => 'textfield',
      '#title' => t('Code'),
  );
  $form['type_project'] = array(
      '#type' => 'select',
      '#options' => array(1 => t('Fixed Price'), 2 => t('Time and material')),
      '#title' => t('Type'),
  );
  $form['category'] = array(
      '#type' => 'select',
      '#options' => array(1 => t('Production'), 2 => t('Sharpen the saw'), 3 => t('Research and development'),
                          4 => t('Leave of Absence'), 5 => t('Another Project') ),
      '#title' => t('Category'),
  );
  $form['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
  );
  $form['status_project'] = array(
      '#type'   => 'radios',
      '#options' => array(1 => t('Started'), 2 => t('Closed')),
      '#title'  => t('Project Status'),
      '#required' => true,
  );

  $options_uid = array();
  $options_uid = get_all_id_account();

  $form['account'] = array(
      '#type' => 'select',
      '#options' => $options_uid,
      '#title' => t('Account name'),
  );

  $options_pid = array();
  $options_pid = get_all_id_people();

  $form['supplier_pm'] = array(
      '#type' => 'select',
      '#options' => $options_pid,
      '#title' => t('Supplier Project Manager'),
  );
  $form['client_pm'] = array(
      '#type' => 'select',
      '#options' => $options_pid,
      '#title' => t('Client Project Manager'),
  );
  $form['account_payee'] = array(
      '#type' => 'select',
      '#options' => $options_pid,
      '#title' => t('Account Payee'),
  );
  $form['initial_mandays'] = array(
      '#type' => 'textfield',
      '#title' => t('Initial manday estimated'),
  );
  $form['current_mandays'] = array(
      '#type' => 'textfield',
      '#title' => t('Current mandays consumed'),
      '#default_value' => 0,
  );
  $form['initial_budget'] = array(
      '#type' => 'textfield',
      '#title' => t('Initial budget estimated'),
  );
  $form['current_budget'] = array(
      '#type' => 'textfield',
      '#title' => t('Current budget consumed'),
      '#default_value' => 0,
  );
  $form['initial_sold_amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Initial sold amount'),
  );
  $form['current_sold_amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Current sold amount'),
  );

  field_attach_form('project', $project, $form, $form_state);

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
    '#submit' => $submit + array('project_admin_add_form_submit'),
  );

  // Show Delete button if we edit task.
  $project_id = entity_id('project' ,$project);
  if (!empty($project_id) && project_access('edit', $project)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('project_admin_add_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'project_admin_add_form_validate';

  return $form;
}

function project_admin_add_form_validate($form, &$form_state) {

}

/**
 * Project submit handler.
 */
function project_admin_add_form_submit($form, &$form_state) {
  $project = $form_state['project'];

  entity_form_submit_build_entity('project', $project, $form, $form_state);

  project_save($project);

  $project_uri = entity_uri('project', $project);

  $form_state['redirect'] = $project_uri['path'];

  drupal_set_message(t('Project %title saved.', array('%title' => entity_label('project', $project))));
}

function project_admin_add_form_submit_delete($form, &$form_state) {
  $project = $form_state['project'];
  $project_uri = entity_uri('project', $project);
  $form_state['redirect'] = $project_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function project_admin_delete_form($form, &$form_state, $project) {
  $form_state['project'] = $project;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['project_type_id'] = array('#type' => 'value', '#value' => entity_id('project' ,$project));
  $project_uri = entity_uri('project', $project);
  return confirm_form($form,
      t('Are you sure you want to delete Project %title?', array('%title' => entity_label('project', $project))),
      $project_uri['path'],
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function project_admin_delete_form_submit($form, &$form_state) {
  $project = $form_state['project'];
  project_delete($project);

  drupal_set_message(t('Project %title deleted.', array('%title' => entity_label('project', $project))));

  $form_state['redirect'] = '<front>';
}