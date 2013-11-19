<?php
/*******************************************************************************
 ********************************* Project Timesheet API's **********************************
******************************************************************************/

/**
 * Load a Project Timesheet.
 */
function project_timesheet_load($tid, $reset = FALSE) {
  $project_timesheet = project_timesheet_load_multiple(array($tid), array(), $reset);
  return reset($project_timesheet);
}

/**
 * Load multiple Project Timesheet based on certain conditions.
 */
function project_timesheet_load_multiple($tids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('project_timesheet', $tids, $conditions, $reset);
}

/**
 * Save Project Timesheet.
 */
function project_timesheet_save($project_timesheet) {
  entity_save('project_timesheet', $project_timesheet);
}

/**
 * Delete single Project Timesheet.
 */
function project_timesheet_delete($project_timesheet) {
  entity_delete('project_timesheet', entity_id('project_timesheet' ,$project_timesheet));
}

/**
 * Delete multiple Project Timesheet.
 */
function project_timesheet_delete_multiple($project_timesheet_ids) {
  entity_delete_multiple('project_timesheet', $project_timesheet_ids);
}

/**
 * Project Timesheet view callback.
 */
function project_timesheet_view($project_timesheet) {
  drupal_set_title(entity_label('project_timesheet', $project_timesheet));
  return entity_view('project_timesheet', array(entity_id('project_timesheet', $project_timesheet) => $project_timesheet), 'full');
}

/*******************************************************************************
 ****************************** Project_Timesheet Type API's ********************************
******************************************************************************/

/**
 * Load Project Timesheet Type.
 */
function project_timesheet_type_load($project_timesheet_type) {
  return project_timesheet_types($project_timesheet_type);
}

/**
 * List of Project Timesheet Types.
 */
function project_timesheet_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('project_timesheet_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Save Project Timesheet type entity.
 */
function project_timesheet_type_save($project_timesheet_type) {
  entity_save('project_timesheet_type', $project_timesheet_type);
}

/**
 * Delete single case type.
 */
function project_timesheet_type_delete($project_timesheet_type) {
  entity_delete('project_timesheet_type', entity_id('project_timesheet_type' ,$project_timesheet_type));
}

/**
 * Delete multiple case types.
 */
function project_timesheet_type_delete_multiple($project_timesheet_type_ids) {
  entity_delete_multiple('project_timesheet_type', $project_timesheet_type_ids);
}