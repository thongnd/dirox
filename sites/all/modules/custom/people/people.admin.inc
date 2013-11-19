<?php
/**
 * Page to select People Type to add new task.
 */
function people_admin_add_page() {
  $items = array();
  foreach (people_types() as $people_type_key => $people_type) {
    $items[] = l(entity_label('people_type', $people_type), PEOPLE_ADMIN_URI . '/add/' . $people_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of People to create.')));
}

/**
 * Add new People page callback.
 */
function people_admin_add($type) {
  $people_type = people_types($type);

  $people = entity_create('people', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('people_type', $people_type))));

  $output = drupal_get_form('people_admin_add_form', $people);

  return $output;
}

function get_all_id_account(){
  $options_uid = array('0' => '- None -');
  $results = db_select('users', 'u,')
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

function get_all_id_people(){
  $options_pid = array('0' => '- None -');
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

function get_all_id_project(){
  $options_ptid = array('0' => '- None -');
  $results = db_select('project', 'pt,')
  ->fields('pt', array('pid','name'))
  ->condition('pt.type', 'project', '=')
  ->orderBy('pt.name','ASC')
  ->execute();

  foreach ($results as $result) {
    $options_ptid[$result->pid] = $result->name;
  }
  return $options_ptid;
}
/**
 * People Form.
 */
function people_admin_add_form($form, &$form_state, $people) {
  $form_state['people'] = $people;

  $options_uid = array();
  $options_uid = get_all_id_account();

  $form['user_id'] = array(
      '#title' => t('User Name'),
      '#type' => 'select',
      '#options' => $options_uid,
      '#default_value' => 0,
  );
  $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
  );
  $form['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#required' => true,
  );
  $form['last_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#required' => true,
  );
  $form['phone'] = array(
      '#type' => 'textfield',
      '#title' => t('Phone Number'),
  );
  $form['address'] = array(
      '#type' => 'textfield',
      '#title' => t('Address'),
  );
  $form['city'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
  );
  $form['zip'] = array(
      '#type' => 'textfield',
      '#title' => t('Zip Code'),
  );
  $form['country'] = array(
      '#type' => 'textfield',
      '#title' => t('Country'),
  );
  $form['job_description'] = array(
      '#type' => 'textfield',
      '#title' => t('Job description'),
      '#required' => true,
  );
  $form['daily_rate'] = array(
      '#type' => 'textfield',
      '#title' => t('Daily rate'),
  );

  $options_pid = array();
  $options_pid = get_all_id_people();

  $form['manager'] = array(
      '#title' => t('Manager'),
      '#type' => 'select',
      '#options' => $options_pid,
  );

  $options_ptid = array();
  $options_ptid = get_all_id_project();

  $form['id_project'] = array(
      '#title' => t('Project(s)'),
      '#type' => 'select',
      '#options' => $options_ptid,
  );

  field_attach_form('people', $people, $form, $form_state);

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
    '#submit' => $submit + array('people_admin_add_form_submit'),
  );

  // Show Delete button if we edit task.
  $people_id = entity_id('people' ,$people);
  if (!empty($people_id) && people_access('edit', $people)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('people_admin_add_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'people_admin_add_form_validate';

  return $form;
}

function people_admin_add_form_validate($form, &$form_state) {

}

/**
 * People submit handler.
 */
function people_admin_add_form_submit($form, &$form_state) {
  $people = $form_state['people'];

  entity_form_submit_build_entity('people', $people, $form, $form_state);

  people_save($people);

  $people_uri = entity_uri('people', $people);

  $form_state['redirect'] = $people_uri['path'];

  drupal_set_message(t('People %title saved.', array('%title' => entity_label('people', $people))));
}

function people_admin_add_form_submit_delete($form, &$form_state) {
  $people = $form_state['people'];
  $people_uri = entity_uri('people', $people);
  $form_state['redirect'] = $people_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function people_admin_delete_form($form, &$form_state, $people) {
  $form_state['people'] = $people;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['people_type_id'] = array('#type' => 'value', '#value' => entity_id('people' ,$people));
  $people_uri = entity_uri('people', $people);
  return confirm_form($form,
      t('Are you sure you want to delete People %title?', array('%title' => entity_label('people', $people))),
      $people_uri['path'],
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function people_admin_delete_form_submit($form, &$form_state) {
  $people = $form_state['people'];
  people_delete($people);

  drupal_set_message(t('People %title deleted.', array('%title' => entity_label('people', $people))));

  $form_state['redirect'] = '<front>';
}
