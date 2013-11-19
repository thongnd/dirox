<?php
/**
 * Week type delete form.
 */
function week_type_form_delete_confirm($form, &$form_state, $week_type) {
  $form_state['week_type'] = $week_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['week_type_id'] = array('#type' => 'value', '#value' => entity_id('week_type', $week_type));
  return confirm_form($form,
      t('Are you sure you want to delete Week type %title?', array('%title' => entity_label('week_type', $week_type))),
      WEEK_URI . '/' . entity_id('week_type' ,$week_type),
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Week type delete form submit handler.
 */
function week_type_form_delete_confirm_submit($form, &$form_state) {
  $week_type = $form_state['week_type'];
  week_type_delete($week_type);

  watchdog('week_type', '@type: deleted %title.', array('@type' => $week_type->type, '%title' => $week_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $week_type->type, '%title' => $week_type->label)));

  $form_state['redirect'] = WEEK_TYPE_ADMIN_URI;
}

/**
 * Generates the Week type editing form.
 */
function week_type_form($form, &$form_state, $week_type, $op = 'edit') {

  if ($op == 'clone') {
    $week_type->label .= ' (cloned)';
    $week_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $week_type->label,
    '#description' => t('The human-readable name of this Week type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($week_type->type) ? $week_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $week_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'week_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this Week type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($week_type->description) ? $week_type->description : '',
    '#description' => t('Description about the Week type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Week type'),
    '#weight' => 40,
  );

  if (!$week_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Week type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('week_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing Week.
 */
function week_type_form_submit(&$form, &$form_state) {
  $week_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  week_type_save($week_type);

  // Redirect user back to list of task types.
  $form_state['redirect'] = WEEK_TYPE_ADMIN_URI;
}

function week_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = WEEK_TYPE_ADMIN_URI . '/' . $form_state['week_type']->type . '/delete';
}