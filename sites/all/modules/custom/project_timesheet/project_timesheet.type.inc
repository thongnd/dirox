<?php
/**
 * Project Timesheet type delete form.
 */
function project_timesheet_type_form_delete_confirm($form, &$form_state, $project_timesheet_type) {
  $form_state['project_timesheet_type'] = $project_timesheet_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['project_timesheet_type_id'] = array('#type' => 'value', '#value' => entity_id('project_timesheet_type', $project_timesheet_type));
  return confirm_form($form,
      t('Are you sure you want to delete Project Timesheet type %title?', array('%title' => entity_label('project_timesheet_type', $project_timesheet_type))),
      PROJECT_TIMESHEET_URI . '/' . entity_id('project_timesheet_type' ,$project_timesheet_type),
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Project Timesheet type delete form submit handler.
 */
function project_timesheet_type_form_delete_confirm_submit($form, &$form_state) {
  $project_timesheet_type = $form_state['project_timesheet_type'];
  project_timesheet_type_delete($project_timesheet_type);

  watchdog('project_timesheet_type', '@type: deleted %title.', array('@type' => $project_timesheet_type->type, '%title' => $project_timesheet_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $project_timesheet_type->type, '%title' => $project_timesheet_type->label)));

  $form_state['redirect'] = PROJECT_TIMESHEET_TYPE_ADMIN_URI;
}

/**
 * Generates the Project Timesheet type editing form.
 */
function project_timesheet_type_form($form, &$form_state, $project_timesheet_type, $op = 'edit') {

  if ($op == 'clone') {
    $project_timesheet_type->label .= ' (cloned)';
    $project_timesheet_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $project_timesheet_type->label,
    '#description' => t('The human-readable name of this Project Timesheet type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($project_timesheet_type->type) ? $project_timesheet_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $project_timesheet_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'project_timesheet_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this Project Timesheet type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($project_timesheet_type->description) ? $project_timesheet_type->description : '',
    '#description' => t('Description about the Project Timesheet type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Project Timesheet type'),
    '#weight' => 40,
  );

  if (!$project_timesheet_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Project Timesheet type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('project_timesheet_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing Project Timesheet.
 */
function project_timesheet_type_form_submit(&$form, &$form_state) {
  $project_timesheet_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  project_timesheet_type_save($project_timesheet_type);

  // Redirect user back to list of task types.
  $form_state['redirect'] = PROJECT_TIMESHEET_TYPE_ADMIN_URI;
}

function project_timesheet_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = PROJECT_TIMESHEET_TYPE_ADMIN_URI . '/' . $form_state['project_timesheet_type']->type . '/delete';
}