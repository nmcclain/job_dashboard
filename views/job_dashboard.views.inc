<?php

/**
 * @file
 * Views hooks implemented for the Job Dashboard module.
 */


/**
 * Implements hook_views_data_alter().
 */
function job_dashboard_views_data_alter(&$data) {
  $data['node']['job_dashboard_webform_lastsubmission'] = array(
    'field' => array(
      'title' => t('Last Webform Submission'),
      'help' => t('The most recent time a Webform was submitted.'),
      'handler' => 'job_dashboard_handler_field_webform_lastsubmission',
    ),
  );
}

/**
 * Implements hook_views_handlers().
 */
function job_dashboard_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'job_dashboard') . '/views',
    ),
    'handlers' => array(
      'job_dashboard_handler_field_webform_lastsubmission' => array(
        'parent' => 'views_handler_field',
        'file' => 'job_dashboard_handler_field_webform_lastsubmission.inc',
      ),
    ),
  );
}



