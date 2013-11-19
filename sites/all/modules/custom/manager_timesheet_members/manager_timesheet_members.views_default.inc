<?php
/**
 * Implements hook_views_default_views().
 */
function manager_timesheet_members_views_default_views() {
  $view = new view;
  $view->name = 'manager_timesheet_members';
  $view->description = 'Manager Timesheet Members';
  $view->tag = 'default';
  $view->base_table = 'manager_timesheet_members';
  $view->human_name = 'Manager Timesheet Members';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manager Timesheet Members';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = FALSE;
  $handler->display->display_options['header']['area']['content'] = '<a href="'.MANAGER_TIMESHEET_MEMBERS_ADMIN_URI.'/add">Add Manager Timesheet Members</a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Field: Task: Task ID */
  $handler->display->display_options['fields']['mtmid']['id'] = 'mtmid';
  $handler->display->display_options['fields']['mtmid']['table'] = 'manager_timesheet_members';
  $handler->display->display_options['fields']['mtmid']['field'] = 'mtmid';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = MANAGER_TIMESHEET_MEMBERS_ADMIN_URI;
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Manager Timesheet Members';
  $handler->display->display_options['menu']['weight'] = '0';

  $views['manager_timesheet_members'] = $view;

  return $views;
}