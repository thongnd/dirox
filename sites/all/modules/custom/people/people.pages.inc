<?php
/*******************************************************************************
 ********************************* People API's **********************************
******************************************************************************/

/**
 * Load a People.
 */
function people_load($pid, $reset = FALSE) {
  $people = people_load_multiple(array($pid), array(), $reset);
  return reset($people);
}

/**
 * Load multiple People based on certain conditions.
 */
function people_load_multiple($pids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('people', $pids, $conditions, $reset);
}

/**
 * Save People.
 */
function people_save($people) {
  entity_save('people', $people);
}

/**
 * Delete single People.
 */
function people_delete($people) {
  entity_delete('people', entity_id('people' ,$people));
}

/**
 * Delete multiple People.
 */
function people_delete_multiple($people_ids) {
  entity_delete_multiple('people', $people_ids);
}

/**
 * People view callback.
 */
function people_view($people) {
  drupal_set_title(entity_label('people', $people));
  return entity_view('people', array(entity_id('people', $people) => $people), 'full');
}

/*******************************************************************************
 ****************************** People Type API's ********************************
******************************************************************************/

/**
 * Load People Type.
 */
function people_type_load($people_type) {
  return people_types($people_type);
}

/**
 * List of People Types.
 */
function people_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('people_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Save People type entity.
 */
function people_type_save($people_type) {
  entity_save('people_type', $people_type);
}

/**
 * Delete single case type.
 */
function people_type_delete($people_type) {
  entity_delete('people_type', entity_id('people_type' ,$people_type));
}

/**
 * Delete multiple case types.
 */
function people_type_delete_multiple($people_type_ids) {
  entity_delete_multiple('people_type', $people_type_ids);
}