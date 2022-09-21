
INSERT INTO transaction_codes VALUES ( 'COC', 'Change of contract', 'FRA' );
INSERT INTO transaction_codes VALUES ( 'OOL', 'Objects on loan', 'FRA' );
INSERT INTO transaction_codes VALUES ( 'MS', 'Medical service', 'FRA' );
DELETE FROM transaction_codes WHERE name = 'Deduction' AND country = 'FRA';
UPDATE transaction_codes SET name = 'Leave of absence and return to work' WHERE name = 'Leave of Absence' AND country = 'FRA';
UPDATE transaction_codes SET name = 'Separation' WHERE name = 'Seperation';
UPDATE transaction_codes SET name = 'Pension Enrolment' WHERE name = 'Pension Enrolement';

