-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Wanted to see all the reports.
SELECT * FROM crime_scene_reports;

-- Find reports on Humphrey St. REPORT: Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
--Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
SELECT * FROM crime_scene_reports WHERE street = 'Humphrey Street';

--Find interviews of the 3 witnesses
SELECT transcript, day FROM interviews WHERE transcript LIKE '%bakery%';

-- Found:
-- 1) Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

-- 2) I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
-- I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

-- 3) As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call,
-- I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Checking bakery's security logs
 SELECT license_plate FROM bakery_security_logs
 WHERE activity = 'exit' AND year = 2021 AND month = 7 AND day = 28 AND minute >= 15 AND minute <= 25;

 -- Match license plates in previous query with list of names.
 SELECT name, license_plate FROM people WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE activity = 'exit' AND year = 2021 AND month = 7 AND day = 28 AND minute >= 15 AND minute <= 25
 );

 -- Check ATM transactions and compare with previous list of names. All names from the Bakery's security log which are not found
 -- in the names of withdrawn-transactions at the crimes date and time will not be returned; hence, suspect list is narrowed down to
 -- returned names.
 SELECT name FROM (
   SELECT name FROM people WHERE license_plate IN (
      SELECT license_plate FROM bakery_security_logs
      WHERE activity = 'exit' AND year = 2021 AND month = 7 AND day = 28 AND minute >= 15 AND minute <= 25))
   WHERE name IN (
      SELECT DISTINCT name FROM people
      JOIN bank_accounts ON people.id = bank_accounts.person_id
      JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
      WHERE atm_location = 'Leggett Street' AND transaction_type = 'withdraw' AND year = 2021 AND month = 7 AND day = 28
   );

-- Check who made a phone call from the previous list. Result: Only Diana and Bruce remain suspects

SELECT name AS caller_names, caller, receiver FROM phone_calls
JOIN people ON people.phone_number = phone_calls.caller
WHERE year = 2021 AND month = 7 AND day = 28 and duration < 60;

-- Focus only on Diana's and Bruce's phone calls

SELECT name AS caller_names, caller, receiver FROM phone_calls
JOIN people ON people.phone_number = phone_calls.caller
WHERE (name = 'Bruce' OR name = 'Diana') AND
year = 2021 AND month = 7 AND day = 28 and duration < 60;

-- Find names of Bruce's and Diana's call receivers.
SELECT name, phone_number FROM people
WHERE phone_number = '(375) 555-8161'
OR phone_number = '(725) 555-3243';

-- Bruce (phone num (367) 555-5533) called (375) 555-8161, which belongs to Robin
-- Diana (phone num (770) 555-1861) called (725) 555-3243, which belongs to Philip

-- Find all flights in the morning of the 29. Fiftyville's airport id is 8.
-- Flight information: (id)36 | 8 (50ville) | 4 (destination) | 2021 | 7 | 29 | 8 | 20
SELECT * FROM flights
WHERE year = 2021 AND month = 7 AND day = 29;

-- Find what id corresponds to id 4. Result: corresponds to LaGuardia (New York City)
SELECT full_name FROM airports WHERE id = 4;

-- Find the name of all people in flight 36 from 50ville to LaGuardia
SELECT name FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON flights.id = passengers.flight_id
WHERE flights.id = 36;

-- Bruce is in this flight, and he called Robin, so Diana is no longer a suspect
-- and Bruce is the probable thief, alongisde Robin as his accomplice. 