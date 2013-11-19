<?php
/**
 * Manager Timesheet class.
 */
class ManagerTimesheet extends Entity {
  protected function defaultLabel() {
    //return $this->title;
  }
  protected function defaultUri() {
    return array('path' => MANAGER_TIMESHEET_URI . '/' . $this->identifier());
  }
}

class ManagerTimesheetController extends EntityAPIController {
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('manager_timesheet', $entity);

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

/**
 * Task Type class.
 */
class ManagerTimesheetType extends Entity {
  public $type;
  public $label;
  public $weight = 0;
  public function __construct($values = array()) {
    parent::__construct($values, 'manager_timesheet_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

class ManagerTimesheetTypeController extends EntityAPIControllerExportable {
  public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'description' => '',
    );
    return parent::create($values);
  }

  /**
   * Save Task Type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see _http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }
}

/**
 * UI controller for Manager Timesheet Type.
 */
class ManagerTimesheetTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Manager Timesheet types.';
    return $items;
  }
}