-- Keep a log of any SQL queries you execute as you solve the mystery.

--Check crime report
SELECT *
FROM crime_scene_reports
WHERE year = 2021
    AND month = 7
    AND day = 28
    AND street = "Humphrey Street";

--Check transcript of intv from witnesses
SELECT transcript,name
FROM interviews
WHERE transcript
    LIKE "%Bakery%";

--Check transactions according to leads from intv
SELECT id, transaction_type, amount, account_number
FROM atm_transactions
WHERE year = 2021
    AND month = 7
    AND day = 28
    AND atm_location = "Leggett Street";

--Check suspect's bank account name
SELECT account_number,name
FROM people
    JOIN bank_accounts
    ON people.id = bank_accounts.person_id
WHERE account_number IN(
    SELECT account_number
    FROM atm_transactions
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND atm_location = "Leggett Street");

--Deduces suspects according to transaction lead & license plate lead
SELECT account_number,name
FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
    WHERE account_number IN(
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2021
            AND month = 7
            AND day = 28
            AND atm_location = "Leggett Street")
    AND license_plate IN(
        SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021
            AND month = 7
            AND day =28);

--Narrows down suspects according to transaction lead, license plate lead, phone calls lead and flight lead
SELECT account_number,name , license_plate
FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
    WHERE account_number IN(
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2021
            AND month = 7
            AND day = 28
            AND atm_location = "Leggett Street"
            AND transaction_type = "withdraw")
    AND license_plate IN(
        SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021
            AND month = 7
            AND day =28
            AND hour = 10
            AND minute < 30)
    AND phone_number IN(
        SELECT caller
        FROM phone_calls
        WHERE year = 2021
            AND month = 7
            AND day = 28
            AND duration < 60)
    AND passport_number IN(
        SELECT passport_number
        FROM passengers
        WHERE flight_id IN(
            SELECT id
            FROM flights
            WHERE year = 2021
                AND month = 7
                AND day = 29
                AND origin_airport_id IN(
                    SELECT id
                    FROM airports
                    WHERE city = "Fiftyville")
                AND hour = 8));

--Check suspect's accomplice
SELECT name
FROM people
WHERE phone_number IN(
    SELECT receiver
    FROM phone_calls
    WHERE phone_number IN(
        SELECT receiver
        FROM phone_calls
        WHERE caller IN(
            SELECT phone_number
            FROM people
            WHERE name = "Bruce")
            AND year = 2021
            AND month = 7
            AND day = 28
            AND duration < 60)

-- Check destination
SELECT city
FROM airports
WHERE id IN (
    SELECT destination_airport_id
    FROM flights
    WHERE year = 2021
        AND month = 7
        AND day = 29
        AND origin_airport_id IN(
            SELECT id
            FROM airports
            WHERE city = "Fiftyville")
        AND hour = 8);
