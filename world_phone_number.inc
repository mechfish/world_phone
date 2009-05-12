<?php
// $Id$

/**
 * @file
 *
 * Define a class to represent an international phone number.
 */

/**
 * An international phone number.
 *
 * - New WorldPhoneNumber objects should not be created directly using "new". Call
 *   WorldPhoneNumber::create() instead.
 *   
 * - Both the number and the country it is for may be independently specified
 *    when a WorldPhoneNumber is created.
 *
 * - If the specified number has a country code, and it disagrees with the
 *    specified country, the country specified by the number's country code
 *    will take precedence. See the country_source() method for more details.
 */
class WorldPhoneNumber {
  private $specified_number;
  private $number;
  private $specified_country;
  private $country;
  private $source;

  /**
   * Private constructor. Don't call this directly, or this WorldPhoneNumber's
   * country setting may be incorrect. Use WorldPhoneNumber::create() instead.
   */
  function __construct($specified_number, $specified_country) {
    $this->specified_number = $specified_number;
    $this->specified_country = $specified_country;
    $this->number = NULL;
    $this->country = NULL;
    $this->source = NULL;
  }

  /**
   * Create a new WorldPhoneNumber, ensuring that its country is correctly specified.
   *
   * @param $specified_number
   *   A string containing a phone number, possibly with a country code included in the string.
   * @param $specified_country
   *   (Optional) A two-character ISO 3166-1a2 code for a country. This may be
   *   used if the number doesn't already include a country code.
   * @return
   *   A new WorldPhoneNumber object.
   */
  static function create($specified_number, $specified_country = NULL) {
    $wpn = new WorldPhoneNumber($specified_number, $specified_country);
    $wpn->determine_country();
    return $wpn;
  }
  
  /**
   * Compute the correct number and country from the specified_number and
   * specified_country contained in this WorldPhoneNumber.
   */
  function determine_country() {
    $parsed = WorldPhoneNumber::parse_number($this->specified_number);
    $this->number = $parsed['remainder'];
    $number_ccode = $parsed['country_code'];
    $number_country = world_phone_country_from_country_code($number_ccode);
    $country_ccode = world_phone_country_code_from_country($this->specified_country);

    if ($number_ccode && $country_ccode && $number_ccode == $country_ccode) {
      $this->country = $this->specified_country;
      $this->source = 'redundant';
    }
    elseif ($number_country && (! empty($this->specified_country)) 
            && ($this->specified_country != '-1')) {
      $this->country = $number_country;
      $this->source = 'conflict';
    }
    elseif ($number_country) {
      $this->country = $number_country;
      $this->source = 'number';
    }
    elseif (empty($this->specified_country) || $this->specified_country == '-1') {
      $this->country = variable_get('world_phone_default_country', 'US');
      $this->source = 'default';
    }
    else {
      $this->country = $this->specified_country;
      $this->source = 'country';
    }
  }

  /**
   * Return the method that was used to determine this WorldPhoneNumber's country.
   *
   * @return
   *   One of the following strings:
   *     - 'redundant': The number includes a country code which matched the
   *        specified country, so the specified country was used.
   *     - 'conflict': The number's country code disagrees with the specified
   *        country, so the number's country was assumed to be correct.
   *     - 'number': The number includes a country code and no $country argument was
   *        provided, so the number's code was used.
   *     - 'country': The number does not include a country code, so the $country
   *        argument was used.
   *     - 'default': The number contains no country code and no $country argument was
   *        provided, so the sitewide default country was used.
   */
  function country_source() {
    return $this->source;
  }

  /**
   * Return the number portion of this WorldPhoneNumber, without country code.
   */
  function get_number() {
    return $this->number;
  }
  
  /**
   * Return the country.
   */
  function get_country() {
    return $this->country;
  }

  /**
   * Return this phone number together with a country code prefix.
   */
  function get_number_with_country_code() {
    if ($this->country && $this->number) {
      return '+' . world_phone_country_code_from_country($this->country) . ' ' . $this->number;
    }
    else {
      // return the number in its original form
      return $this->specified_number;
    }    
  }

  /**
   * Divide the given string, representing a phone number, into country-code and remainder
   * portions. If the number does not begin with a "+", it will be assumed to not have a
   * country code.
   *
   * @param $number
   *   A string representation of a phone number, optionally containing a country code.
   * @return
   *   An array with the following keys:
   *     - 'country_code': The country code from the beginning of the number, or NULL if there
   *       is no recognized country code at the beginning of the number.
   *     - 'remainder': The part of the number left over after the country code is removed.
   */
  static function parse_number($number) {
    $ccode = NULL;
    $remainder = $number;
    if (strlen($remainder) > 0 && _world_phone_number_has_intl_prefix($number)) {
      // take the first few digits at a time and match against the phone codes.
      // see http://en.wikipedia.org/wiki/List_of_country_calling_codes for explanation of this algorithm        
      $ccode_digit_count = 0;
      do {
        $result = _world_phone_find_digits($remainder, 1);
        if (strlen($result['digits']) < 1) {
          // couldn't find another digit
          $ccode_exists = FALSE;
          break;
        }
        $ccode = $ccode . $result['digits'];
        $ccode_digit_count++;
        $remainder = $result['remainder'];
        $ccode_exists = world_phone_country_code_exists($ccode);
      } while ((! $ccode_exists) && $ccode_digit_count < 3 && strlen($remainder) > 0);
      if (! $ccode_exists) {
        $ccode = NULL;
        $remainder = $number;
      }
      elseif ($ccode == '1' && strlen($number) >= 4) {
        // code 1 is special: it might be the start of a four-digit code
        $result = _world_phone_find_digits($number, 0, 4);
        $long_ccode = $result['digits'];
        if (strlen($long_ccode) == 4 &&
            world_phone_country_code_exists($long_ccode)) {
          $ccode = $long_ccode;
          $remainder = $result['remainder'];
        }
      }
    }
    return array(
      'country_code' => $ccode,
      // take remainder of number, stripping leading and trailing dashes and spaces
      'remainder' => rtrim(ltrim($remainder, ' -)'), ' -('),
    );
  }
}


/**
 * HELPER FUNCTIONS
 */

/**
 * Search through $string character by character, accumulating digits into a variable, until $num_digits have been found.
 * Return the digits that were accumulated and a 'remainder' consisting of the string that is left over after
 * the final digit in $num_digits has been found.
 *
 * Example: f('1 plus 2 is 3', 1) => digits: "1", remainder: " plus 2 is 3"
 *          f('1 plus 2 is 3', 2) => digits: "12", remainder: " is 3"
 */
function _world_phone_find_digits($string, $num_digits) {
  $pos = 0;
  $digit_count = 0;
  $digits = '';
  while ($digit_count < $num_digits) {
    $chr = substr($string, $pos, 1);
    if (preg_match('/[0-9]/', $chr)) {
      $digits = $digits . $chr;
      $digit_count++;
    }
    $pos++;
    if ($pos >= strlen($string)) {
      return array('digits' => $digits, 'remainder' => '');
    }
  }
  return array('digits' => $digits, 'remainder' => substr($string, $pos));
}

/**
 * Return TRUE if the given phone number begins with a '+', indicating that it
 * includes a country code.
 */
function _world_phone_number_has_intl_prefix($number) {
  return preg_match('/^\s*\+/', $number);
}
