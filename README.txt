$Id$

CONTENTS OF THIS FILE
---------------------

  * Introduction
  * Installation
  * Usage
  * Known Problems
  * Acknowledgements


INTRODUCTION
------------

Current Maintainer: Michael F. Booth (aka mechfish) <mike@literate-devices.com>

World Phone module implements a CCK field for entering international
phone numbers with country codes. Administrators may add one or more
phone number fields to any node type. When entering phone numbers, a
site's users may select country codes from a dropdown list of
countries, or include them with the typed number itself by prefixing
it with a "+" sign.

When displayed, numbers can be shown with or without their country
codes. The module can also suppress the display of a country code if
it matches the default country code for the site. (So that, for
example, a site based in the UK can be configured to display country
codes only for non-UK phone numbers.)

This module includes basic support for every country code defined in
ITU-T recommendation E.164 (see the country_codes.inc file for
more details).

For phone numbers which are restricted to a single country, you may
wish to use the Phone CCK module instead.


INSTALLATION
------------

1. Copy the module directory to your site's modules directory
(typically sites/all/modules or sites/[sitename]/modules.

2. Enable the module in Admin > Modules

3. [Optional, but strongly recommended] Install jQuery Update module
for proper IE support and widget behavior.


USAGE
-----

Once the module is enabled, you may use CCK to add a World Phone field
to any node type.

World Phone fields are edited using a special widget which contains a
dropdown menu for selecting the country code. If the user types a "+"
at the start of the phone number, the dropdown will go away and the
country code will be taken from the first few digits of the number.


KNOWN PROBLEMS
--------------

There is as yet no easy way to make the country code dropdowns look
good in IE6. Most likely an entirely new Javascript widget for
presenting the dropdowns will have to be developed.


ACKNOWLEDGEMENTS
----------------

This module was originally developed by:

  RealCME, Inc. 
  http://www.realcme.com

Thanks to all the authors of Phone module and the other Drupal modules
that inspired this one.

For the moment, this module incorporates Sam Collett's jQuery Selectboxes 
module, under its GPL license. See http://www.texotela.co.uk/

