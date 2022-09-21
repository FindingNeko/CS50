-- A schema call to find out what we can learn

.schema

-- Digging further into the crime scene report seems as good a place as any
-- to get started, I'll query just that day.

SELECT *
  FROM crime_scene_reports
 WHERE year=2021
   AND month=7
   AND day=28;

-- The crime scene report mentions the theft occuring at 10:15am at the Humphrey
-- Street bakery, there were 3 witnesses present who were interviewed, so I'll
-- look at those interviews next. They occured the same day.

SELECT *
  FROM interviews
 WHERE year=2021
   AND month=7
   AND day=28;

-- A witness saw the thief get in a car in the bakery parking lot within ten
-- minutes of the theft, he said to check the tapes around then.
-- A witness saw the thief withdrawing money on Leggett Street some time before
-- the theft on the monrning it occured.
-- A witness heard the thief say they were taking the earliest flight out of
-- Fiftyville tomorrow (7-29-2021), and tell someone to buy the ticket.
-- I better check those tapes first.

SELECT *
  FROM bakery_security_logs
 WHERE year=2021
   AND month=7
   AND day=28
   AND hour=10
   AND minute BETWEEN 15 AND 25;

-- I got a license plate, but I need more. I'll check the ATM next.

SELECT *
  FROM atm_transactions
 WHERE year=2021
   AND month=7
   AND day=28
   AND atm_location LIKE '%LEGGETT%';

-- I don't see anything useful in the atm transaction yet, lets try the phone
-- records. The thief made a call as they left the bakery.

SELECT *
  FROM phone_calls
 WHERE year=2021
   AND month=7
   AND day=28
   AND duration<60;

-- A few too many phone results to narrow anythig down, how about the flights
-- out of town on the morning after the crime?

SELECT *
  FROM flights
 WHERE year=2021
   AND month=7
   AND day=29
 ORDER BY hour ASC;

-- It looks like the earliest flight out is a 8:20am flight to airport id 4.
-- Let's have a look at the passenger list for this flight.

SELECT *
  FROM passengers
 WHERE flight_id=36;

-- Eight passengers, with seat numbers and passport numbers. Our suspect called
-- an accomplice who purchased the tickets. Lets have a look at the bank account
-- records to see if anyting jumps out.

SELECT *
  FROM bank_accounts;

-- No smoking guns here, but maybe now that we've seen everything we can make a
-- query to solve the mystery!

SELECT DISTINCT name
  FROM people
        JOIN bank_accounts
          ON bank_accounts.person_id=people.id
            JOIN atm_transactions
              ON atm_transactions.account_number=bank_accounts.account_number
        INNER JOIN phone_calls
           ON people.phone_number=phone_calls.caller
        INNER JOIN bakery_security_logs
           ON people.license_plate=bakery_security_logs.license_plate
        INNER JOIN passengers
           ON people.passport_number=passengers.passport_number
        WHERE people.passport_number
        IN (SELECT passport_number
              FROM passengers
             WHERE flight_id=36)
        AND people.license_plate
        IN (SELECT license_plate
              FROM bakery_security_logs
             WHERE year=2021
               AND month=7
               AND day=28
               AND hour=10
               AND minute BETWEEN 15 AND 25)
        AND people.phone_number
        IN (SELECT caller
              FROM phone_calls
             WHERE year=2021
               AND month=7
               AND day=28
               AND duration<60);

-- Ah ha! Our mega query has revealed that BRUCE fits the evidence. He was at
-- the atm, make a short phone call, was spotted leaving the scene, and was
-- booked on the first flight out of town in the morning. Now lets find his
-- accomplice using the information we have about Bruce.

SELECT DISTINCT name
  FROM people
        WHERE phone_number=
              (SELECT receiver
                 FROM phone_calls
                WHERE year=2021
                  AND month=7
                  AND day=28
                  AND duration<60
                  AND caller=
                    (SELECT phone_number
                       FROM people
                      WHERE name='Bruce'));

-- The last piece of the puzzle is where the thief flew off to, all we need to
-- do is check the airport code for number 4.

SELECT *
  FROM airports
 WHERE id=4;

-- Bruce ran to New York City with help from Robin! Holy smokes!