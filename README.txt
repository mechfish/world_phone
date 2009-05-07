/* $Id$ */

World Phone (CCK)
==================

A module which implements a CCK field for entering international phone
numbers with country codes. The user may select country codes from a
dropdown list of countries, or type them into a text box. 

When displayed, numbers can be shown with or without their country
codes, or the country code can be shown only when it differs from the
site's default country code. (So that, e.g., a site based in the UK
can omit country codes for UK-based phone numbers but show them for
other countries' phone numbers.)

As a bonus, if you have an existing Drupal content type in which phone
numbers are stored in a text field, this module allows you to use the
World Phone widget, with country-code dropdowns, to display and edit
that text field.

The module includes hooks (presently undocumented) which will allow it
to be extended to support custom formatting of phone numbers based on
their country of origin.

This was inspired by the original Phone (CCK) module, but it includes
basic support for every country code defined in ITU-T recommendation
E.164 (see the countries/wp_default.inc file, included in this module,
for more details).


REQUIREMENTS
============

This module is designed for Drupal 5 and PHP 5.x. It is not guaranteed
to work properly in earlier versions of PHP, or other versions of
Drupal. The Drupal 6 version is separate, and forthcoming.

The jQuery Update module is required to make the country code
dropdowns work properly and look good, especially in IE7. The module
will run without jQuery Update, but IE support is more primitive and
the widget behaviors will not be correct.

Unfortunately, there seems to be no easy way to make the country code
dropdowns look good in IE6. So far, it has defeated our Javascript
hacks, and cursing does not help. Perhaps future revisions of this
module will contain a better solution.


INSTALLATION
============

1. Copy the module directory to your site's modules directory
(typically sites/all/modules or sites/[sitename]/modules.

2. Enable the module in Admin > Modules

3. [Optional, but strongly recommended] Install jQuery Update module
for proper IE support and widget behavior.


USAGE
=====

Once the module is enabled, you may use CCK to add a World Phone field
to any node type.

For some reason, you may wish to store international phone numbers in
a text field inside of a node. (For example, you might have existing
phone numbers stored in a text field, and you might not wish to figure
out how to migrate them to a new World Phone field.) In this case, you
can use the included formatter to format the text field as an
international phone number. It will be stored as text, but when a user
edits it they will see the special World Phone widget.

World Phone fields are edited using a special widget which contains a
dropdown menu for selecting the country code. If the user types a "+"
at the start of the phone number, the dropdown goes away and the
country code is taken from the first few digits of the number.


SPONSOR
=======

This module was developed by:

  RealCME, Inc. 
  http://www.realcme.com


ACKNOWLEDGEMENTS
================

Thanks to all the authors of Phone module and the other Drupal modules
that inspired this one.

This module makes use of Sam Collett's jQuery Selectboxes module,
under the MIT and/or GPL licenses.  Kudos to him, and see http://www.texotela.co.uk


AUTHOR/MAINTAINER
=================

Michael Booth
mike at literate-devices DOT com
http://www.mechanicalrobotfish.com
