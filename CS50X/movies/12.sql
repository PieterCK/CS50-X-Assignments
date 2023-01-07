SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
WHERE id IN(
    SELECT movie_id FROM stars WHERE person_id IN(
        SELECT id FROM people WHERE name = "Johnny Depp"))AND person_id IN(
            SELECT id FROM people WHERE name = "Helena Bonham Carter");