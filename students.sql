CREATE TABLE stus (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL,
    age INT,
    school VARCHAR(100)
);

INSERT INTO stus (id, username, age, school)
VALUES
    (1, 'zhongli', 3000, 'chinese'),
    (2, 'hutao', 18, 'math'),
    (3, 'ganyu', 200, 'english'),
    (4, 'xiao', 500, 'sports');