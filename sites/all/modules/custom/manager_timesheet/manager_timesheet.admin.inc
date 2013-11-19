<?php
/**
 * Page to select Manager Timesheet Type to add new task.
 */
function manager_timesheet_admin_add_page() {
  $items = array();
  foreach (manager_timesheet_types() as $manager_timesheet_type_key => $manager_timesheet_type) {
    $items[] = l(entity_label('manager_timesheet_type', $manager_timesheet_type), MANAGER_TIMESHEET_ADMIN_URI . '/add/' . $manager_timesheet_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of Manager Timesheet to create.')));
}

/**
 * Add new Manager Timesheet page callback.
 */
function manager_timesheet_admin_add($type) {
  $manager_timesheet_type = manager_timesheet_types($type);

  $manager_timesheet = entity_create('manager_timesheet', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('manager_timesheet_type', $manager_timesheet_type))));

  $output = drupal_get_form('manager_timesheet_admin_add_form', $manager_timesheet);

  return $output;
}

/**
 * Manager Timesheet Form.
 */
function manager_timesheet_admin_add_form($form, &$form_state, $manager_timesheet) {
  $form_state['manager_timesheet'] = $manager_timesheet;

  $options_wid = array();
  $results = db_select('week', 'w,')
  ->fields('w', array('wid'))
  ->condition('w.type', 'week', '=')
  ->orderBy('w.wid')
  ->execute();

  foreach ($results as $result) {
    $options_wid[$result->wid] = $result->wid;
  }

  $form['week'] = array(
      '#title' => t('Week'),
      '#type' => 'select',
      '#options' => $options_wid,
  );
  $form['billable_days'] = array(
      '#type' => 'textfield',
      '#title' => t('Billable days (Based on contract needs) ?'),
  );
  $form['comment'] = array(
      '#type' => 'textarea',
      '#title' => t('Comment'),
  );
  $form['timesheet_status'] = array(
      '#type'   => 'radios',
      '#options' => array(1 => t('Todo'), 2 => t('Submited'), 3 => t('Approved')),
      '#title'  => t('Timesheet Status'),
  );

  field_attach_form('manager_timesheet', $manager_timesheet, $form, $form_state);

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
    '#submit' => $submit + array('manager_timesheet_admin_add_form_submit'),
  );

  // Show Delete button if we edit task.
  $manager_timesheet_id = entity_id('manager_timesheet' ,$manager_timesheet);
  if (!empty($manager_timesheet_id) && manager_timesheet_access('edit', $manager_timesheet)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('manager_timesheet_admin_add_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'manager_timesheet_admin_add_form_validate';

  return $form;
}

function manager_timesheet_admin_add_form_validate($form, &$form_state) {

}

/**
 * Manager Timesheet submit handler.
 */
function manager_timesheet_admin_add_form_submit($form, &$form_state) {
  $manager_timesheet = $form_state['manager_timesheet'];

  entity_form_submit_build_entity('manager_timesheet', $manager_timesheet, $form, $form_state);

  manager_timesheet_save($manager_timesheet);

  $manager_timesheet_uri = entity_uri('manager_timesheet', $manager_timesheet);

  $form_state['redirect'] = $manager_timesheet_uri['path'];

  drupal_set_message(t('Manager Timesheet %title saved.', array('%title' => entity_label('manager_timesheet', $manager_timesheet))));
}

function manager_timesheet_admin_add_form_submit_delete($form, &$form_state) {
  $manager_timesheet = $form_state['manager_timesheet'];
  $manager_timesheet_uri = entity_uri('manager_timesheet', $manager_timesheet);
  $form_state['redirect'] = $manager_timesheet_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function manager_timesheet_admin_delete_form($form, &$form_state, $manager_timesheet) {
  $form_state['manager_timesheet'] = $manager_timesheet;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['manager_timesheet_type_id'] = array('#type' => 'value', '#value' => entity_id('manager_timesheet' ,$manager_timesheet));
  $manager_timesheet_uri = entity_uri('manager_timesheet', $manager_timesheet);
  return confirm_form($form,
      t('Are you sure you want to delete Manager Timesheet %title?', array('%title' => entity_label('manager_timesheet', $manager_timesheet))),
      $manager_timesheet_uri['path'],
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function manager_timesheet_admin_delete_form_submit($form, &$form_state) {
  $manager_timesheet = $form_state['manager_timesheet'];
  manager_timesheet_delete($manager_timesheet);

  drupal_set_message(t('Manager Timesheet %title deleted.', array('%title' => entity_label('manager_timesheet', $manager_timesheet))));

  $form_state['redirect'] = '<front>';
}
