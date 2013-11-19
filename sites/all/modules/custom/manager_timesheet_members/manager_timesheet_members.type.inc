<?php
/**
 * Manager Timesheet Members type delete form.
 */
function manager_timesheet_members_type_form_delete_confirm($form, &$form_state, $manager_timesheet_members_type) {
  $form_state['manager_timesheet_members_type'] = $manager_timesheet_members_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['manager_timesheet_members_type_id'] = array('#type' => 'value', '#value' => entity_id('manager_timesheet_members_type', $manager_timesheet_members_type));
  return confirm_form($form,
      t('Are you sure you want to delete Manager Timesheet Members type %title?', array('%title' => entity_label('manager_timesheet_members_type', $manager_timesheet_members_type))),
      MANAGER_TIMESHEET_MEMBERS_URI . '/' . entity_id('manager_timesheet_members_type' ,$manager_timesheet_members_type),
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Manager Timesheet Members type delete form submit handler.
 */
function manager_timesheet_members_type_form_delete_confirm_submit($form, &$form_state) {
  $manager_timesheet_members_type = $form_state['manager_timesheet_members_type'];
  manager_timesheet_members_type_delete($manager_timesheet_members_type);

  watchdog('manager_timesheet_members_type', '@type: deleted %title.', array('@type' => $manager_timesheet_members_type->type, '%title' => $manager_timesheet_members_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $manager_timesheet_members_type->type, '%title' => $manager_timesheet_members_type->label)));

  $form_state['redirect'] = MANAGER_TIMESHEET_MEMBERS_TYPE_ADMIN_URI;
}

/**
 * Generates the Manager Timesheet Members type editing form.
 */
function manager_timesheet_members_type_form($form, &$form_state, $manager_timesheet_members_type, $op = 'edit') {

  if ($op == 'clone') {
    $manager_timesheet_members_type->label .= ' (cloned)';
    $manager_timesheet_members_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $manager_timesheet_members_type->label,
    '#description' => t('The human-readable name of this Manager Timesheet Members type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($manager_timesheet_members_type->type) ? $manager_timesheet_members_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $manager_timesheet_members_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'manager_timesheet_members_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this Manager Timesheet Members type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($manager_timesheet_members_type->description) ? $manager_timesheet_members_type->description : '',
    '#description' => t('Description about the Manager Timesheet Members type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  if (!$manager_timesheet_members_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('manager_timesheet_members_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing Manager Timesheet Members.
 */
function manager_timesheet_members_type_form_submit(&$form, &$form_state) {
  $manager_timesheet_members_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  manager_timesheet_members_type_save($manager_timesheet_members_type);

  // Redirect user back to list of task types.
  $form_state['redirect'] = MANAGER_TIMESHEET_MEMBERS_TYPE_ADMIN_URI;
}

function manager_timesheet_members_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = MANAGER_TIMESHEET_MEMBERS_TYPE_ADMIN_URI . '/' . $form_state['manager_timesheet_members_type']->type . '/delete';
}