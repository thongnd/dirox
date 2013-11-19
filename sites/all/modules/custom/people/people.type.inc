<?php
/**
 * People type delete form.
 */
function people_type_form_delete_confirm($form, &$form_state, $people_type) {
  $form_state['people_type'] = $people_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['people_type_id'] = array('#type' => 'value', '#value' => entity_id('people_type', $people_type));
  return confirm_form($form,
      t('Are you sure you want to delete People type %title?', array('%title' => entity_label('people_type', $people_type))),
      PEOPLE_URI . '/' . entity_id('people_type' ,$people_type),
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * People type delete form submit handler.
 */
function people_type_form_delete_confirm_submit($form, &$form_state) {
  $people_type = $form_state['people_type'];
  people_type_delete($people_type);

  watchdog('people_type', '@type: deleted %title.', array('@type' => $people_type->type, '%title' => $people_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $people_type->type, '%title' => $people_type->label)));

  $form_state['redirect'] = PEOPLE_TYPE_ADMIN_URI;
}

/**
 * Generates the People type editing form.
 */
function people_type_form($form, &$form_state, $people_type, $op = 'edit') {

  if ($op == 'clone') {
    $people_type->label .= ' (cloned)';
    $people_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $people_type->label,
    '#description' => t('The human-readable name of this People type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($people_type->type) ? $people_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $people_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'people_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this People type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($people_type->description) ? $people_type->description : '',
    '#description' => t('Description about the People type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save People type'),
    '#weight' => 40,
  );

  if (!$people_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete People type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('people_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing People.
 */
function people_type_form_submit(&$form, &$form_state) {
  $people_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  people_type_save($people_type);

  // Redirect user back to list of task types.
  $form_state['redirect'] = PEOPLE_TYPE_ADMIN_URI;
}

function people_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = PEOPLE_TYPE_ADMIN_URI . '/' . $form_state['people_type']->type . '/delete';
}