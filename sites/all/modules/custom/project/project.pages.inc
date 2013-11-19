<?php
/*******************************************************************************
 ********************************* Project API's **********************************
******************************************************************************/

/**
 * Load a Project.
 */
function project_load($pid, $reset = FALSE) {
  $project = project_load_multiple(array($pid), array(), $reset);
  return reset($project);
}

/**
 * Load multiple Project based on certain conditions.
 */
function project_load_multiple($pids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('project', $pids, $conditions, $reset);
}

/**
 * Save Project.
 */
function project_save($project) {
  entity_save('project', $project);
}

/**
 * Delete single Project.
 */
function project_delete($project) {
  entity_delete('project', entity_id('project' ,$project));
}

/**
 * Delete multiple Project.
 */
function project_delete_multiple($project_ids) {
  entity_delete_multiple('project', $project_ids);
}

/**
 * Project view callback.
 */
function project_view($project) {
  drupal_set_title(entity_label('project', $project));
  return entity_view('project', array(entity_id('project', $project) => $project), 'full');
}

/*******************************************************************************
 ****************************** Project Type API's ********************************
******************************************************************************/

/**
 * Load Project Type.
 */
function project_type_load($project_type) {
  return project_types($project_type);
}

/**
 * List of Project Types.
 */
function project_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('project_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Save Project type entity.
 */
function project_type_save($project_type) {
  entity_save('project_type', $project_type);
}

/**
 * Delete single case type.
 */
function project_type_delete($project_type) {
  entity_delete('project_type', entity_id('project_type' ,$project_type));
}

/**
 * Delete multiple case types.
 */
function project_type_delete_multiple($project_type_ids) {
  entity_delete_multiple('project_type', $project_type_ids);
}