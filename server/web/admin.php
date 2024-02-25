<html>
<head>
    <title>PS4MEMAPI Web Admin</title>
    <style>
        body {
            background-color: #F5F5F5;
        }
        
        table {
            border-collapse: collapse;
        }

        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <center>
        <h2>PS4MEMAPI Web Admin</h2>

        <?php
        $link = mysqli_connect("127.0.0.1", "ps4trainer", "hk294ienekmal", "ps4trainer");

        if(mysqli_connect_errno()) {
            die("mysqli_connect_errno: " . mysqli_connect_error());
        }

        if(!strcmp($_SERVER["REQUEST_METHOD"], "POST")) {
            if(isset($_POST["removeid"])) {
                $query = "DELETE FROM cheats WHERE ID=" . mysqli_real_escape_string($link, $_POST["removeid"]);
                $result = mysqli_query($link, $query);
                if(!result) {
                    die("mysqli_query: " . mysqli_error($link));
                }

                printf("<p>removed %s</p>\n", $_POST["removeid"]);
            } else if(isset($_POST["titleid"]) && isset($_POST["region"]) && isset($_POST["version"]) && isset($_POST["cheatdata"])) {
                $query = "INSERT INTO cheats (titleid, region, version, cheatdata) VALUES (\"" . mysqli_real_escape_string($link, $_POST["titleid"]) . "\", \"" . mysqli_real_escape_string($link, $_POST["region"]) . "\", \"" . mysqli_real_escape_string($link, $_POST["version"]) . "\", \"" . mysqli_real_escape_string($link, $_POST["cheatdata"]) . "\")";
                $result = mysqli_query($link, $query);
                if(!result) {
                    die("mysqli_query: " . mysqli_error($link));
                }

                if(!mysqli_commit($link)) {
                    die("mysqli_commit failed while adding");
                }

                printf("<p>added %s</p>\n", $_POST["titleid"]);
            }
        }
        ?>
        
        <table>
            <tr><th>ID</th><th>Title ID</th><th>Region</th><th>Version</th><th>Cheat Data</th></tr>
            <?php
                $result = mysqli_query($link, "SELECT * FROM cheats");
                while($row = mysqli_fetch_row($result)) {
                    printf("<tr>");
                    printf("<td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td>", $row[0], $row[1], $row[2], $row[3], $row[4]);
                    printf("</tr>\n");
                }
                mysqli_free_result($result);
            ?>
        </table>

        </br>

        <form method="post">
            <input name="titleid">
            <input name="region">
            <input name="version">
            <input name="cheatdata">
            <button>Add Cheat</button>
        </form>

        </br>

        <form method="post">
            <input name="removeid">
            <button>Remove Cheat</button>
        </form>
    </center>
</body>
</html>

<?php
mysqli_close($link);
?>
