<?php if (!empty($fields)): ?>
  <dl>
    <?php foreach ($fields as $label => $info): ?>
      <dt><?php echo $label; ?></dt>
      <dd><?php echo $info; ?></dd>
    <?php endforeach; ?>
  </dl>
<?php endif; ?>