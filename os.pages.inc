<?php

/**
 * Implements hook_form().
 */
function os_form() {
  $form['#validate'] = array('os_validator');
  $form['#submit'] = array('os_submit');
  $form['login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
  );
  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('Prénom'),
    '#required' => TRUE,
  );
  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Nom'),
    '#required' => TRUE,
  );
  $form['society'] = array(
    '#type' => 'select',
    '#title' => t('Société'),
    '#options' => array(
      1 => 'Société A',
      2 => 'Société B',
      3 => 'société C',
    ),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Validate'),
  );

  return $form;
}

function os_validator($form, &$form_state) {
  if (isset($form_state['values']['login'])) {
    $query = 'SELECT * FROM {os} WHERE LOWER(login)=LOWER(:log)';

    $result = db_query($query, array(
      ':log' => $form_state['values']['login']
    ))->fetchAssoc();

    if ($error = user_validate_name($form_state['values']['login'])) {
      form_set_error('login', $error);
    }
    elseif (!empty($result)) {
      $message = t('The name %name is already taken.', array('%name' => $form_state['values']['login']));
      form_set_error('login', $message);
    }

    if ($error = user_validate_mail($form_state['values']['email'])) {
      form_set_error('email', $error);
    }
  }
}

function os_submit($form, &$form_state) {
  $insert = db_insert('os')->fields(array(
    'login' => $form_state['values']['login'],
    'email' => $form_state['values']['email'],
    'firstname' => $form_state['values']['firstname'],
    'lastname' => $form_state['values']['lastname'],
    'society' => $form_state['values']['society'],
    'created' => time(),
  ))->execute();

  if ($insert) {
    drupal_set_message(t('Votre demande a été enregistré.'));
  }
}