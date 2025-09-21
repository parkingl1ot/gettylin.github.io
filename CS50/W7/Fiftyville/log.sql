
-- Keep a log of any SQL queries you execute as you solve the mystery.

-- read the crime scene reports
SELECT * FROM crime_scene_reports
    WHERE month = 7
    AND day = 28
    AND street = 'Humphrey Street';

-- read relevant interviews
SELECT * FROM interviews
    WHERE month = 7
    AND day = 28
    AND transcript LIKE '%bakery%';

-- find people exitting the bakery parking lot between 10:15-10:25am
-- and withdrawing money on Leggett Street earlier that day
-- and making phonecall for less than a minute
SELECT name, receiver FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN phone_calls ON phone_calls.caller = people.phone_number
    WHERE license_plate in
        (SELECT license_plate FROM bakery_security_logs
            WHERE month = 7
            AND day = 28
            AND hour = 10
            AND minute >= 15 AND minute <=25
            AND activity = 'exit')
    AND account_number in
        (SELECT account_number FROM atm_transactions
            WHERE month = 7
            AND day = 28
            AND atm_location = 'Leggett Street')
    AND phone_number in
        (SELECT caller FROM phone_calls
            WHERE month = 7
            AND day = 28
            AND duration < 60)
    AND duration < 60;

-- the city they escaped to: NEW YORK CITY
SELECT city FROM airports
    WHERE id =
        (SELECT flights.destination_airport_id FROM flights, airports
            WHERE flights.origin_airport_id = airports.id
            AND city = 'Fiftyville'
            AND year = 2024
            AND month = 7
            AND day = 29
            ORDER BY hour, minute ASC
            LIMIT 1);

-- accomplice: Doris
SELECT name from people
    WHERE phone_number in
        (SELECT receiver FROM  phone_calls
            WHERE month = 7
            AND day = 28
            AND duration < 60)
    AND passport_number in
        (SELECT passport_number FROM passengers
         WHERE flight_id =
            (SELECT flights.id FROM flights, airports
            WHERE flights.origin_airport_id = airports.id
            AND city = 'Fiftyville'
            AND year = 2024
            AND month = 7
            AND day = 29
            ORDER BY hour, minute ASC
            LIMIT 1));


-- hopefully the real accomplice
SELECT name, phone_number from people
    WHERE phone_number in
        (SELECT receiver FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN phone_calls ON phone_calls.caller = people.phone_number
    WHERE license_plate in
        (SELECT license_plate FROM bakery_security_logs
            WHERE month = 7
            AND day = 28
            AND hour = 10
            AND minute >= 15 AND minute <=25
            AND activity = 'exit')
    AND account_number in
        (SELECT account_number FROM atm_transactions
            WHERE month = 7
            AND day = 28
            AND atm_location = 'Leggett Street')
    AND phone_number in
        (SELECT caller FROM phone_calls
            WHERE month = 7
            AND day = 28
            AND duration < 60)
    AND duration < 60);

SELECT name FROM people
    WHERE passport_number in
        (SELECT passport_number FROM passengers
            WHERE flight_id = 36);
