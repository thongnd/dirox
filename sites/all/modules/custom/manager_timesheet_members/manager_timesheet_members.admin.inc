<?php
/**
 * Page to select Manager Timesheet Members Type to add new task.
 */
function manager_timesheet_members_admin_add_page() {
  $items = array();
  foreach (manager_timesheet_members_types() as $manager_timesheet_members_type_key => $manager_timesheet_members_type) {
    $items[] = l(entity_label('manager_timesheet_members_type', $manager_timesheet_members_type), MANAGER_TIMESHEET_MEMBERS_ADMIN_URI . '/add/' . $manager_timesheet_members_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of Manager Timesheet Members to create.')));
}

/**
 * Add new Manager Timesheet Members page callback.
 */
function manager_timesheet_members_admin_add($type) {
  $manager_timesheet_members_type = manager_timesheet_members_types($type);

  $manager_timesheet_members = entity_create('manager_timesheet_members', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('manager_timesheet_members_type', $manager_timesheet_members_type))));

  $output = drupal_get_form('manager_timesheet_members_admin_add_form', $manager_timesheet_members);

  return $output;
}

/**
 * Manager Timesheet Members Form.
 */
function manager_timesheet_members_admin_add_form($form, &$form_state, $manager_timesheet_members) {
  $form_state['manager_timesheet_members'] = $manager_timesheet_members;

  $options_mtid = array();
  $results = db_select('manager_timesheet', 'm,')
  ->fields('m', array('mtid'))
  ->condition('m.type', 'manager_timesheet', '=')
  ->orderBy('m.mtid')
  ->execute();

  foreach ($results as $result) {
    $options_mtid[$result->mtid] = $result->mtid;
  }

  $form['id_timesheet'] = array(
      '#title' => t('ID Timesheet'),
      '#type' => 'select',
      '#options' => $options_mtid,
  );

  $options_pid = array();
  $results = db_select('people', 'p,')
  ->fields('p', array('pid','first_name','last_name'))
  ->condition('p.type', 'people', '=')
  ->orderBy('p.pid')
  ->execute();

  foreach ($results as $result) {
    $options_pid[$result->pid] = $result->first_name.' '.$result->last_name;
  }

  $form['id_member'] = array(
      '#title' => t('ID Member'),
      '#type' => 'select',
      '#options' => $options_pid,
  );
  $form['billed_days'] = array(
      '#type' => 'textfield',
      '#title' => t('Billed days this week'),
  );
  $form['staff_on_leave'] = array(
      '#type' => 'textfield',
      '#title' => t('Staff on leave this week.'),
  );
  $form['staff_helping'] = array(
      '#type' => 'textfield',
      '#title' => t('Staff helping on my project this week.'),
  );
  $form['staff_sts'] = array(
      '#type' => 'textfield',
      '#title' => t('Staff working on Sharpening the Saw.'),
  );
  $form['staff_rnd'] = array(
      '#type' => 'textfield',
      '#title' => t('Staff working on Research and Development this week.'),
  );
  $form['staff_other'] = array(
      '#type' => 'textfield',
      '#title' => t("Staff working on someone else's projects this week."),
  );

  field_attach_form('manager_timesheet_members', $manager_timesheet_members, $form, $form_state);

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
    '#submit' => $submit + array('manager_timesheet_members_admin_add_form_submit'),
  );

  // Show Delete button if we edit task.
  $manager_timesheet_members_id = entity_id('manager_timesheet_members' ,$manager_timesheet_members);
  if (!empty($manager_timesheet_members_id) && manager_timesheet_members_access('edit', $manager_timesheet_members)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('manager_timesheet_members_admin_add_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'manager_timesheet_members_admin_add_form_validate';

  return $form;
}

function manager_timesheet_members_admin_add_form_validate($form, &$form_state) {

}

/**
 * Manager Timesheet Members submit handler.
 */
function manager_timesheet_members_admin_add_form_submit($form, &$form_state) {
  $manager_timesheet_members = $form_state['manager_timesheet_members'];

  entity_form_submit_build_entity('manager_timesheet_members', $manager_timesheet_members, $form, $form_state);

  manager_timesheet_members_save($manager_timesheet_members);

  $manager_timesheet_members_uri = entity_uri('manager_timesheet_members', $manager_timesheet_members);

  $form_state['redirect'] = $manager_timesheet_members_uri['path'];

  drupal_set_message(t('Manager Timesheet Members %title saved.', array('%title' => entity_label('manager_timesheet_members', $manager_timesheet_members))));
}

function manager_timesheet_members_admin_add_form_submit_delete($form, &$form_state) {
  $manager_timesheet_members = $form_state['manager_timesheet_members'];
  $manager_timesheet_members_uri = entity_uri('manager_timesheet_members', $manager_timesheet_members);
  $form_state['redirect'] = $manager_timesheet_members_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function manager_timesheet_members_admin_delete_form($form, &$form_state, $manager_timesheet_members) {
  $form_state['manager_timesheet_members'] = $manager_timesheet_members;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['manager_timesheet_members_type_id'] = array('#type' => 'value', '#value' => entity_id('manager_timesheet_members' ,$manager_timesheet_members));
  $manager_timesheet_members_uri = entity_uri('manager_timesheet_members', $manager_timesheet_members);
  return confirm_form($form,
      t('Are you sure you want to delete Manager Timesheet Members %title?', array('%title' => entity_label('manager_timesheet_members', $manager_timesheet_members))),
      $manager_timesheet_members_uri['path'],
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function manager_timesheet_members_admin_delete_form_submit($form, &$form_state) {
  $manager_timesheet_members = $form_state['manager_timesheet_members'];
  manager_timesheet_members_delete($manager_timesheet_members);

  drupal_set_message(t('Manager Timesheet Members %title deleted.', array('%title' => entity_label('manager_timesheet_members', $manager_timesheet_members))));

  $form_state['redirect'] = '<front>';
}
