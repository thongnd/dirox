<?php
/**
 * Implements hook_views_default_views().
 */
function project_timesheet_views_default_views() {
  $view = new view;
  $view->name = 'project_timesheet';
  $view->description = 'Project Timesheet';
  $view->tag = 'default';
  $view->base_table = 'project_timesheet';
  $view->human_name = 'Project Timesheet';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Project Timesheet';
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
  $handler->display->display_options['header']['area']['content'] = '<a href="'.PROJECT_TIMESHEET_ADMIN_URI.'/add">Add Project Timesheet</a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;

  /* Field: Task: Task ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'project_timesheet';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = PROJECT_TIMESHEET_ADMIN_URI;
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Project Timesheet';
  $handler->display->display_options['menu']['weight'] = '0';

  $views['project_timesheet'] = $view;

  return $views;
}