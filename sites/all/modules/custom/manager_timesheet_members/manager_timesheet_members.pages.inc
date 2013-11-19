<?php
/*******************************************************************************
 ********************************* Manager Timesheet Members API's **********************************
******************************************************************************/

/**
 * Load a Manager Timesheet Members.
 */
function manager_timesheet_members_load($mtmid, $reset = FALSE) {
  $manager_timesheet_members = manager_timesheet_members_load_multiple(array($mtmid), array(), $reset);
  return reset($manager_timesheet_members);
}

/**
 * Load multiple Manager Timesheet Members based on certain conditions.
 */
function manager_timesheet_members_load_multiple($mtmids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('manager_timesheet_members', $mtmids, $conditions, $reset);
}

/**
 * Save Manager Timesheet Members.
 */
function manager_timesheet_members_save($manager_timesheet_members) {
  entity_save('manager_timesheet_members', $manager_timesheet_members);
}

/**
 * Delete single Manager Timesheet Members.
 */
function manager_timesheet_members_delete($manager_timesheet_members) {
  entity_delete('manager_timesheet_members', entity_id('manager_timesheet_members' ,$manager_timesheet_members));
}

/**
 * Delete multiple Manager Timesheet Members.
 */
function manager_timesheet_members_delete_multiple($manager_timesheet_members_ids) {
  entity_delete_multiple('manager_timesheet_members', $manager_timesheet_members_ids);
}

/**
 * Manager Timesheet Members view callback.
 */
function manager_timesheet_members_view($manager_timesheet_members) {
  drupal_set_title(entity_label('manager_timesheet_members', $manager_timesheet_members));
  return entity_view('manager_timesheet_members', array(entity_id('manager_timesheet_members', $manager_timesheet_members) => $manager_timesheet_members), 'full');
}

/*******************************************************************************
 ****************************** Manager Timesheet Members Type API's ********************************
******************************************************************************/

/**
 * Load Manager Timesheet Members Type.
 */
function manager_timesheet_members_type_load($manager_timesheet_members_type) {
  return manager_timesheet_members_types($manager_timesheet_members_type);
}

/**
 * List of Manager Timesheet Members Types.
 */
function manager_timesheet_members_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('manager_timesheet_members_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Save Manager Timesheet Members type entity.
 */
function manager_timesheet_members_type_save($manager_timesheet_members_type) {
  entity_save('manager_timesheet_members_type', $manager_timesheet_members_type);
}

/**
 * Delete single case type.
 */
function manager_timesheet_members_type_delete($manager_timesheet_members_type) {
  entity_delete('manager_timesheet_members_type', entity_id('manager_timesheet_members_type' ,$manager_timesheet_members_type));
}

/**
 * Delete multiple case types.
 */
function manager_timesheet_members_type_delete_multiple($manager_timesheet_members_type_ids) {
  entity_delete_multiple('manager_timesheet_members_type', $manager_timesheet_members_type_ids);
}