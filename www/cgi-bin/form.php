<?php
header('Content-Type: text/html; charset=UTF-8');
?>
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Résultat PHP (GET/POST)</title>
</head>
<body>

    <h1>Réception des données (PHP)</h1>

    <?php
    if ($_SERVER['REQUEST_METHOD'] === 'GET') {
        echo "<h2>Méthode : GET</h2>";

        if (!empty($_GET['username_get'])) {
            $username = htmlspecialchars($_GET['username_get']);
            echo "<p><strong>Nom (username_get) reçu :</strong> " . $username . "</p>";
        } else {
            echo "<p>Aucun nom reçu via GET.</p>";
        }
    }

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        echo "<h2>Méthode : POST</h2>";

        if (!empty($_POST['username_post'])) {
            $username = htmlspecialchars($_POST['username_post']);
            echo "<p><strong>Nom (username_post) reçu :</strong> " . $username . "</p>";
        } else {
            echo "<p>Aucun nom reçu via POST.</p>";
        }
    }

    if ($_SERVER['REQUEST_METHOD'] !== 'GET' && $_SERVER['REQUEST_METHOD'] !== 'POST') {
        echo "<p>Accès direct. Veuillez soumettre un formulaire.</p>";
    }
    ?>

</body>
</html>
