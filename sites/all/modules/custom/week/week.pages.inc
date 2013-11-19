<?php

/*******************************************************************************
 ********************************* Week API's **********************************
******************************************************************************/

/**
 * Load a Week.
 */
function week_load($wid, $reset = FALSE) {
  $week = week_load_multiple(array($wid), array(), $reset);
  return reset($week);
}

/**
 * Load multiple Week based on certain conditions.
 */
function week_load_multiple($wids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('week', $wids, $conditions, $reset);
}

/**
 * Save Week.
 */
function week_save($week) {
  entity_save('week', $week);
}

/**
 * Delete single Week.
 */
function week_delete($week) {
  entity_delete('week', entity_id('week' ,$week));
}

/**
 * Delete multiple Week.
 */
function week_delete_multiple($week_ids) {
  entity_delete_multiple('week', $week_ids);
}

/**
 * Week view callback.
 */
function week_view($week) {
  drupal_set_title(entity_label('week', $week));
  return entity_view('week', array(entity_id('week', $week) => $week), 'full');
}

/*******************************************************************************
 ****************************** Week Type API's ********************************
******************************************************************************/

/**
 * Load Week Type.
 */
function week_type_load($week_type) {
  return week_types($week_type);
}

/**
 * List of Week Types.
 */
function week_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('week_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Save Week type entity.
 */
function week_type_save($week_type) {
  entity_save('week_type', $week_type);
}

/**
 * Delete single case type.
 */
function week_type_delete($week_type) {
  entity_delete('week_type', entity_id('week_type' ,$week_type));
}

/**
 * Delete multiple case types.
 */
function week_type_delete_multiple($week_type_ids) {
  entity_delete_multiple('week_type', $week_type_ids);
}