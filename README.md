`email_validator`
================

Drupal module to provide email address checker

Config at admin/config/email-validator
Debugging tool at admin/report/email-validator

Writes into `email_validator` table. The result codes are:

0. all good
1. Host denied us access to SMTP
2. invalid host
3. host not accepting mail
4. host not accepting mail from us, for this email address.
5. Something went wrong in SMTP chat. Counts as positive pass.
6. Unable to obtain a definiet yes or no. Counts as positive.
