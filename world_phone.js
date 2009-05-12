// $Id$

$(document).ready(function() {
  // don't run without jQuery Update installed
  if (jQuery.fn.jquery === '1.0.4') {
    return;
  }

  // hide the blank option
  $("select.wp-widget-country option[value='-1']").hide();

  // compensate for IE7+ troubles
  if ($.browser.msie === true &&
      $.browser.version.substr(0,1) !== '6') {
    $("select.wp-widget-country").focus(function(e) {
      $(this).css('width', '16em')
      .css('position', 'absolute')
      .parents('.wp-widget').find('.wp-widget-number').css('width', '19em');
    }).blur(function(e) {
      $(this).css('width', '6em')
      .css('position', 'static')
      .parents('.wp-widget').find('.wp-widget-number').css('width', '12em');
    });
  }

  // compensate for IE6 troubles
  if ($.browser.msie === true &&
      $.browser.version.substr(0,1) === '6') {
    $("select.wp-widget-country").each(function() {
      var selected_option = $(this).selectedValues();
      var o = this.options;
      var oL = o.length;
      // create an array for sorting
      var sA = [];
      for (var i = 0; i<oL; i++) {
        sA[i] = {
          v: o[i].value,
	  t: o[i].text
        };
      }
      sA.sort(
        function(o1, o2) {
          // if options are the same, no sorting is needed
          if (o1.v == o2.v) return 0;
          return o1.v < o2.v ? -1 : 1;
        }
      );
      // change the options to match the sort array
      for (i = 0; i<oL; i++) {
        o[i].text = sA[i].t;
        o[i].value = sA[i].v;
      }
      $(this).selectOptions(selected_option, true);
    });
  }

  // hide the select box when user types a + in front of a phone number
  // do not do this if jQuery Update is not installed
  $('input.wp-widget-number').keyup(function(e) {
    var had_ccode = $(this).data('had_ccode');
    var has_ccode = /^\s*\+/.test($(this).val());
    $(this).data('had_ccode', has_ccode);
    if (had_ccode == undefined || (had_ccode ? ! has_ccode : has_ccode)) {
      // we're initializing, or ccode status has changed
      var cwidget = $(this).parents('div.wp-widget').find('select.wp-widget-country');
      if (has_ccode) {
        $(this).data('old_selected_values', $(cwidget).selectedValues()).addClass('with-ccode');
        $(cwidget).selectOptions('-1', true).addClass('blank');
      }
      else {
        var old_val = $(this).data('old_selected_values');
        if (old_val == '-1') {
          old_val = Drupal.settings.world_phone_default_country;
        }
        $(this).removeClass('with-ccode');
        $(cwidget).selectOptions(old_val, true).removeClass('blank');
      }
    }
  }).keyup();
});