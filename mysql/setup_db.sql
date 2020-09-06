---- Create user
-- INSERT INTO mysql.user (User, Host, authentication_string, ssl_cipher, x509_issuer, x509_subject)
--    VALUES ('chat', 'localhost', PASSWORD('nctfuchat0905hc'), '', '', '');
-- FLUSH PRIVILEGES;

---- Create database
CREATE DATABASE chat;
-- GRANT ALL PRIVILEGES ON chat.* TO chat@localhost;
-- FLUSH PRIVILEGES;

---- Create table: users
-- DROP TABLE chat.users;
CREATE TABLE chat.users (username CHAR(10), password CHAR(15), icon CHAR(15),
                         account_type INT, last_active INT);
INSERT INTO chat.users VALUES ('BBBBB', 'nc7FU_W3LL',  'man01.png', 0, 1);
INSERT INTO chat.users VALUES ('CCCCC', '????????',    'man02.png', 1, 1);
INSERT INTO chat.users VALUES ('DDDDD', '???????',     'man04.png', 1, 2);
INSERT INTO chat.users VALUES ('Lily',  'wh0_15_111y', 'man03.png', 1, 2);
INSERT INTO chat.users VALUES ('autis', 'cut3',        'default.png', 2, 0); -- 放進去誰嗎?
INSERT INTO chat.users VALUES ('david', 'Hand50M3',    'default.png', 2, 0);
INSERT INTO chat.users VALUES ('yen',   '84D_9uy',     'default.png', 2, 0);

---- Create table: message
-- DROP TABLE chat.message;
CREATE TABLE chat.message (isMessage BOOL, deleted BOOL, name VARCHAR(15), time INT,
                           call_duration CHAR(11), msg TEXT);

INSERT INTO chat.message VALUES (1, 0, 'BBBBB',   5, 0, '事情都到這地步了，該怎麼辦....');
INSERT INTO chat.message VALUES (1, 0, 'CCCCC',   5, 0, '還是我們明天來討論一下，大家一起想想怎麼處理比較好？');
INSERT INTO chat.message VALUES (1, 0, 'BBBBB',   5, 0, '好');

INSERT INTO chat.message VALUES (1, 0, 'BBBBB',   4, 0, '有聲音嗎？');
INSERT INTO chat.message VALUES (1, 0, 'DDDDD',   4, 0, '有了');
INSERT INTO chat.message VALUES (0, 0, '',        4, 27, '');
INSERT INTO chat.message VALUES (1, 0, 'BBBBB',   4, 0, '今天很感謝有你們的幫忙！！TAT');
INSERT INTO chat.message VALUES (1, 0, 'Lily',    4, 0, '(o^∀^)');

INSERT INTO chat.message VALUES (1, 0, 'CCCCC',   2, 0, '話說我們要不換個平台啊，總覺得在這邊講話有被監聽的感覺 (還是我的錯覺 ^_^;) ');
INSERT INTO chat.message VALUES (1, 0, 'Lily',    2, 0, '同意+1');
INSERT INTO chat.message VALUES (1, 1, 'BBBBB',   2, 0, '改用這個好了，密碼老樣子');
INSERT INTO chat.message VALUES (1, 1, 'BBBBB',   2, 0, '? qqgnoe 的 url 咧');
