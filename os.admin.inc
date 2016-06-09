<?php

function os_list_form() {
  $limit = 10;

  $status = array(
    '0' => t('En attente'),
    '1' => t('Validé'),
    '2' => t('Rejeté'),
  );

  $header = array(
    'login' => array('data' => t('login'), 'field' => 'so.login'),
    'email' => t('Email'),
    'firstname' => array('data' => t('Prénom'), 'field' => 'so.firstname'),
    'lastname' => array('data' => t('Nom'), 'field' => 'so.lastname'),
    'created' => array(
      'data' => t('Date de la demande'),
      'field' => 'so.created'
    ),
    'status' => t('Status'),
  );


  $query = db_select('os', 'so');
  user_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(so.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query->fields('so', array(
    'nid',
    'login',
    'email',
    'firstname',
    'lastname',
    'created',
    'status'
  ))->limit($limit);

  $query
    ->orderByHeader($header)
    ->setCountQuery($count_query);

  $result = $query->execute();

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $status,
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $options = array();
  foreach ($result as $request) {

    $options[$request->nid] = array(
      'login' => $request->login,
      'email' => $request->email,
      'firstname' => $request->firstname,
      'lastname' => $request->lastname,
      'created' => format_interval(REQUEST_TIME - $request->created),
      'status' => $status[$request->status],
    );

    $disabled = array();
    if ($request->status != 0) {
      $disabled[] = $request->nid;
    }
  }

  $form['accounts'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    //'#disabled' => $disabled,
    '#empty' => t('No people available.'),
  );

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

function os_list_form_submit($form, &$form_state) {
  if ($form_state['values']['operation'] != 0) {
    $count = 0;
    $values = array();
    $where = ' WHERE nid IN ( ';
    foreach ($form_state['values']['accounts'] as $nid => $checked) {
      if (!empty($checked)) {
        $where .= ':v' . $nid . ',';
        $values[':v' . $nid] = $nid;
        $count++;
      }
    }
    $where = substr($where, 0, strlen($where) - 1);
    $where .= ' )';

    if ($count > 0) {
      $query = 'UPDATE {os} SET status=:status' . $where;
      $values = array_merge(
        array(':status' => $form_state['values']['operation']),
        $values
      );


      if (db_query($query, $values)) {
        drupal_set_message(t('Les demandes ont été mises à jour.'));
      }
    }
  }
}