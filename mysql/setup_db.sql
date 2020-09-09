---- Create user
CREATE USER chat@'localhost' IDENTIFIED BY 'nctfuchathc';
CREATE USER chat@'%' IDENTIFIED BY 'nctfuchathc';
FLUSH PRIVILEGES;

---- Create database
CREATE DATABASE chat;
GRANT SELECT ON chat.* TO chat@'localhost';
GRANT SELECT ON chat.* TO chat@'%';
FLUSH PRIVILEGES;

---- Create table: users
-- DROP TABLE chat.users;
CREATE TABLE chat.users (username VARCHAR(50), password CHAR(15), icon CHAR(15),
                         account_type INT, last_active INT);
INSERT INTO chat.users VALUES ('小辣椒', 'bl42kkk_31d0ww',  'man01.png', 0, 1);
INSERT INTO chat.users VALUES ('霍根', 'ri244_ma5000n',    'man02.png', 1, 1);
INSERT INTO chat.users VALUES ('蜘蛛人', 'R3D_6u4rd14n',     'man04.png', 1, 2);
INSERT INTO chat.users VALUES ('Lily',  'wh0_15_111y', 'man03.png', 1, 2);
INSERT INTO chat.users VALUES ('autis', 'cut3',        'default.png', 2, 0); -- 放進去誰嗎?
INSERT INTO chat.users VALUES ('david', 'Hand50M3',    'default.png', 2, 0);
INSERT INTO chat.users VALUES ('yen',   '84D_9uy',     'default.png', 2, 0);

---- Create table: message
-- DROP TABLE chat.message;
CREATE TABLE chat.message (isMessage BOOL, deleted BOOL, name VARCHAR(50), time INT,
                           call_duration CHAR(11), msg TEXT);

INSERT INTO chat.message VALUES (1, 0, '小辣椒',   5, 0, '事情都到這地步了，該怎麼辦....');
INSERT INTO chat.message VALUES (1, 0, '霍根',   5, 0, '還是我們明天來討論一下，大家一起想想怎麼處理比較好？');
INSERT INTO chat.message VALUES (1, 0, '小辣椒',   5, 0, '好');

INSERT INTO chat.message VALUES (1, 0, '小辣椒',   4, 0, '有聲音嗎？');
INSERT INTO chat.message VALUES (1, 0, '蜘蛛人',   4, 0, '有了');
INSERT INTO chat.message VALUES (0, 0, '',        4, 27, '');
INSERT INTO chat.message VALUES (1, 0, '小辣椒',   4, 0, '今天很感謝有你們的幫忙！！TAT');
INSERT INTO chat.message VALUES (1, 0, 'Lily',    4, 0, '(o^∀^)');

INSERT INTO chat.message VALUES (1, 0, '霍根',   2, 0, '話說我們要不換個平台啊，總覺得在這邊講話有被監聽的感覺 (還是我的錯覺 ^_^;) ');
INSERT INTO chat.message VALUES (1, 0, 'Lily',    2, 0, '同意+1');
INSERT INTO chat.message VALUES (1, 1, '小辣椒',   2, 0, '改用這個好了，密碼老樣子');
INSERT INTO chat.message VALUES (1, 1, '小辣椒',   2, 0, '? qqgnoe 的 url 咧');
