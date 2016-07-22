<?php
/**
 * Provides a helper function to modify user input to forms.
 *
 * Sometimes this is useful and DRY.
 *
 * Version 1.0
 */
// Allow possibility of using this in a few projects.
if (!function_exists('form_mangle_user_input')) {
  /**
   * Change the user-entered value.
   *
   * @param Array $element. The element in the $form array, e.g. $element from
   * an element_validate function.
   * @param mixed $value. The value you want to change it to.
   * @param &Array $form_state.
   */
  function form_mangle_user_input(&$element, $value, &$form_state) {

    /**
     * Follow parents down array, then set value.
     *
     * @param array $ptr top level.
     * @param array $parents
     * @param mixed $value. If an array its values will be merged in, replacing
     * any value for keys that already exist. Otherwise it is used as the value
     * directly.
     */
    $walk = function (&$ptr, $parents, $value) {
      foreach ($parents as $_) {
        if (!isset($ptr[$_])) {
          return;
        }
        $ptr = & $ptr[$_];
      }
      if (is_array($value)) {
        foreach ($value as $k=>$v) {
          $ptr[$k] = $v;
        }
      }
      else {
        $ptr = $value;
      }
    };

    // Correct the $element's value. This is trivial but enables code below the
    // call to this function to work with $element['#value'] as normal.
    $element['#value'] = $value;

    // This sets it in form_state values. Drupal has its own version of this,
    // but we use ours for consistnecy within this function. Equivalent to
    // Drupal's form_set_value($element, $value, $form_state);
    $walk($form_state['values'], $element['#parents'], $value);

    // Fix the 'input'. This helps the case of rebuild since a rebuild means the
    // input values are copied into the form element values again, so our
    // changes would otherwise be lost.
    $walk($form_state['input'], $element['#parents'], $value);

    // Now fix the 'complete form' value. This is so that any subsequent
    // element_validate functions get the new value.
    $walk($form_state['complete form'], $element['#parents'], ['#value' => $value]);
  }

}
