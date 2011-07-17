<?php

/**
 * @file
 * Views handler to display the 
 */

/**
 * Field handler to present the submission count of a node to the user.
 */
class job_dashboard_handler_field_webform_lastsubmission extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['nid'] = 'nid';
    # $this->additional_fields['type'] = 'type';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['label'] = array('default' => 'Last Submission', 'translatable' => TRUE);
    return $options;
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    global $user;

    $output = _job_dashboard_handler_field_lastsubmission_fetch_result_code($values->{$this->aliases['nid']});

    return $output;
  }
}

function _job_dashboard_handler_field_lastsubmission_fetch_result_code($nid) {
  $node = node_load($nid);

  // check for webform module and load submissions inc file
  if (!module_load_include('inc', 'webform', 'includes/webform.submissions')) {
    $output = 'Webform module not found!';
    watchdog('job_dashboard', 'Job View error (lastsubmission field): '.$output);
    return $output;
  }

  // load CSS for styling the field - TODO: is this okay?
  drupal_add_css(drupal_get_path('module', 'job_dashboard') . '/job_dashboard.css');

  // lookup most recent webform submission for this node
  $filters = array('nid' => $nid);
  $submissions = webform_get_submissions($filters, NULL, 0); // TODO: is it possible to load most recent submission without loading ALL submissions?

  if (count($submissions) > 0) {
    $last_submission = array_pop($submissions);
    $mapping = _job_webform_component_mapping($node);
    // the time of this submission
    $output = check_plain(date('Y-m-d H:i:s', $last_submission->submitted));

    // if there is a result_code webform field for this submission, wrap it in a style-able div
    if (isset($last_submission->data[$mapping['result_code']]['value'][0])) {
      $wrapper_top = '<div class="';
      if ($last_submission->data[$mapping['result_code']]['value'][0] == 1) { // TODO: improve this for more result codes
        $wrapper_top .= "job-success";
      } else {
        $wrapper_top .= "job-fail";
      }
      $wrapper_top .= '">';
      $output = $wrapper_top . $output . '</div>';
    }

  } else {
    $output = '<div class="job-never">' . t('Never') . '</div>';
  }


  return $output;
}
