`email_validator`
================

Drupal module to provide email address checker functionality for module writers.

Emails can be checked for syntax, DNS and SMTP. You get to configure what level
you want to test to. Lookups are cached (once a day per email) and flood control
is used to ban single IPs that make lots of lookups (you can configure when to
block them, defaults to after 3 failed attempts).

- Config at `admin/config/email-validator`
- Debugging tool at `admin/report/email-validator`
- Demo form at `admin/report/email-validator/demo`


## Installation

Normal module installation but then you **must save the configuration form**.

Then have a play with the demo form and try the debugger (both under Reports menu).


## Usage

There are two ways to use this module: automatic form integration and manually
using the checker.


### To check an email on a form.

In your form's definition for the email field, add the following:

    $form = [
      // ...
      'some_email_field' => [
        // ... all your existing properties ...
        '#form_id' => $form['#form_id'],
        '#pre_render' => ['email_validator_pre_render'],
        '#element_validate' => ['email_validator_element_validate'],
      ],
      // ...
    ];

Those three parts are all required. It should work with other `#pre_render` and
`#element_validate` functions too, I've shown it here with just the required ones.

That's it. The form won't submit with suspect emails without user re-entering
the email as confirmation.

### To use the checker directly.

    $result = email_validator_run_check($some_email);

Then check `$result['confidence']` for one of:

- `EMAIL_VALIDATOR_UNDELIVERABLE` this is not going to work out. Syntax invalid,
  or the domain has no DNS (not even an A record).
- `EMAIL_VALIDATOR_UNSURE` we got some errors. Might be temporary. Might not.
  Your call.
- `EMAIL_VALIDATOR_OK` all good.

## Configuration.

You can exclude domains by simple glob patterns (`fnmatch`) from the SMTP
interrogation. Some MTAs consider attempts that don't end up in an email being
sent suspicous, so if you know of these hosts you might want to exclude them.
The default config suggests some Microsoft domains because experience shows that
they are very quick to block IPs pretty unjustifiably, but I don't have it on any
authority that they don't like these checks.

The return path is important, and should be set up correctly.

## Other notes

Writes into `email_validator` table. The result codes are:

    0 - all good
    1 - Host denied us access to SMTP
    2 - invalid host
    3 - host not accepting mail
    4 - host not accepting mail from us, for this email address.
    5 - Something went wrong in SMTP chat. Counts as positive pass.
    6 - Unable to obtain a definiet yes or no. Counts as positive.

