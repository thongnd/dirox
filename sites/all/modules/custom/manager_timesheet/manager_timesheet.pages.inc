<?php
/*******************************************************************************
 ********************************* Manager Timesheet API's **********************************
******************************************************************************/

/**
 * Load a Manager Timesheet.
 */
function manager_timesheet_load($mtid, $reset = FALSE) {
  $manager_timesheet = manager_timesheet_load_multiple(array($mtid), array(), $reset);
  return reset($manager_timesheet);
}

/**
 * Load multiple Manager Timesheet based on certain conditions.
 */
function manager_timesheet_load_multiple($mtids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('manager_timesheet', $mtids, $conditions, $reset);
}

/**
 * Save Manager Timesheet.
 */
function manager_timesheet_save($manager_timesheet) {
  entity_save('manager_timesheet', $manager_timesheet);
}

/**
 * Delete single Manager Timesheet.
 */
function manager_timesheet_delete($manager_timesheet) {
  entity_delete('manager_timesheet', entity_id('manager_timesheet' ,$manager_timesheet));
}

/**
 * Delete multiple Manager Timesheet.
 */
function manager_timesheet_delete_multiple($manager_timesheet_ids) {
  entity_delete_multiple('manager_timesheet', $manager_timesheet_ids);
}

/**
 * Manager Timesheet view callback.
 */
function manager_timesheet_view($manager_timesheet) {
  drupal_set_title(entity_label('manager_timesheet', $manager_timesheet));
  return entity_view('manager_timesheet', array(entity_id('manager_timesheet', $manager_timesheet) => $manager_timesheet), 'full');
}

/*******************************************************************************
 ****************************** Manager Timesheet Type API's ********************************
******************************************************************************/

/**
 * Load Manager Timesheet Type.
 */
function manager_timesheet_type_load($manager_timesheet_type) {
  return manager_timesheet_types($manager_timesheet_type);
}

/**
 * List of Manager Timesheet Types.
 */
function manager_timesheet_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('manager_timesheet_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Save Manager Timesheet type entity.
 */
function manager_timesheet_type_save($manager_timesheet_type) {
  entity_save('manager_timesheet_type', $manager_timesheet_type);
}

/**
 * Delete single case type.
 */
function manager_timesheet_type_delete($manager_timesheet_type) {
  entity_delete('manager_timesheet_type', entity_id('manager_timesheet_type' ,$manager_timesheet_type));
}

/**
 * Delete multiple case types.
 */
function manager_timesheet_type_delete_multiple($manager_timesheet_type_ids) {
  entity_delete_multiple('manager_timesheet_type', $manager_timesheet_type_ids);
}