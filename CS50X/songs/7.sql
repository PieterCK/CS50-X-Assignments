SELECT AVG(energy) AS avg_energy_Drake FROM songs WHERE artist_id IN (SELECT id FROM artists WHERE name = "Drake");