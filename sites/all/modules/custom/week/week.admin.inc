<?php
/**
 * Page to select Week Type to add new task.
 */
function week_admin_add_page() {
  $items = array();
  foreach (week_types() as $week_type_key => $week_type) {
    $items[] = l(entity_label('week_type', $week_type), WEEK_ADMIN_URI . '/add/' . $week_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of Week to create.')));
}

/**
 * Add new Week page callback.
 */
function week_admin_add($type) {
  $week_type = week_types($type);

  $week = entity_create('week', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('week_type', $week_type))));

  $output = drupal_get_form('week_admin_add_form', $week);

  return $output;
}

/**
 * Week Form.
 */
function week_admin_add_form($form, &$form_state, $week) {
  $form_state['week'] = $week;

  $form['year'] = array(
      '#type' => 'textfield',
      '#title' => t('Year'). ' :',
      '#required' => true,
  );
  $form['week_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of the week'). ' :',
      '#required' => true,
  );
  $form['week_working_days'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of the working days in the week'). ' :',
      '#required' => true,
  );
  $dateformat = 'd-m-Y';
  $timestamp = REQUEST_TIME;

  $d_start = new DateTime('@'.$timestamp);
  $d_start->setTimezone(new DateTimeZone(date_default_timezone()));
  $start_value = $d_start->format($dateformat);
  $form['week_first_day'] = array(
      '#type' => 'date_select',
      '#title' => t('First day in the week (DD/MM/YYYY)'). ' :',
      '#date_format'         => $dateformat,
      '#default_value'       => $start_value,
      '#date_label_position' => 'none',
      '#after_build'         => array('week_date_select_build'),
      '#required' => true,
  );

  field_attach_form('week', $week, $form, $form_state);

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
    '#submit' => $submit + array('week_admin_add_form_submit'),
  );

  // Show Delete button if we edit task.
  $week_id = entity_id('week' ,$week);
  if (!empty($week_id) && week_access('edit', $week)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('week_admin_add_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'week_admin_add_form_validate';

  return $form;
}

function week_date_select_build($form_element, &$form_state){
  unset($form_element['day']['#options']['']);
  unset($form_element['month']['#options']['']);
  unset($form_element['year']['#options']['']);
  return $form_element;
}

function week_admin_add_form_validate($form, &$form_state) {

}

/**
 * Week submit handler.
 */
function week_admin_add_form_submit($form, &$form_state) {
  $week = $form_state['week'];

  $form_state['values']['week_first_day'] = date_create_from_format('Y-m-d', $form_state['values']['week_first_day'])->getTimestamp();
  entity_form_submit_build_entity('week', $week, $form, $form_state);

  week_save($week);

  $week_uri = entity_uri('week', $week);

  $form_state['redirect'] = $week_uri['path'];

  drupal_set_message(t('Week %title saved.', array('%title' => entity_label('week', $week))));
}

function week_admin_add_form_submit_delete($form, &$form_state) {
  $week = $form_state['week'];
  $week_uri = entity_uri('week', $week);
  $form_state['redirect'] = $week_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function week_admin_delete_form($form, &$form_state, $week) {
  $form_state['week'] = $week;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['week_type_id'] = array('#type' => 'value', '#value' => entity_id('week' ,$week));
  $week_uri = entity_uri('week', $week);
  return confirm_form($form,
      t('Are you sure you want to delete Week %title?', array('%title' => entity_label('week', $week))),
      $week_uri['path'],
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function week_admin_delete_form_submit($form, &$form_state) {
  $week = $form_state['week'];
  week_delete($week);

  drupal_set_message(t('Week %title deleted.', array('%title' => entity_label('week', $week))));

  $form_state['redirect'] = '<front>';
}
