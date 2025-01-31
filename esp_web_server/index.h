const char index_html[] PROGMEM = R"ESPHTML(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP32 Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">

    <style>
        /* Default Colors*/
        :root {
            --anchor-color: blue;
            --font-color: black;
            --background-color: white;
            --primary-color: darkblue;
            --secondary-color: darkgreen;
            --active-color: green;
            --icon-color: black;
        }

        html {
            box-sizing: border-box;
        }

        *,
        *:before,
        *:after {
            box-sizing: inherit;
        }

        body {
            margin: 0;
            font-size: 100%;
            font-family: 'Courier New', Courier, monospace;
            background-color: var(--background-color);
        }

        a {
            color: var(--font-color);
            text-decoration: none;
        }

        a:hover {
            color: var(--active-color);
        }

        button {
            border: 2px solid black;
            background-color: white;
            font-family: 'Courier New', Courier, monospace;
            font-size: 1.4rem;
            font-weight: bold;
            padding-left: 1rem;
            padding-right: 1rem;
        }

        button:hover {
            cursor: pointer;
            background-color: var(--active-color);
        }

        label {
            font-size: 1.4rem;
            font-weight: bold;
        }

        input {
            border: 2px solid black;
            background-color: white;
            width: 10rem;
            font-family: 'Courier New', Courier, monospace;
            font-size: 1.4rem;
            font-weight: bold;
        }

        /* App Styles */
        .app {
            border: 4px solid black;
            width: 1200px;
            height: 800px;
            margin: auto;
            margin-top: 2rem;
            padding: 1rem;
        }

        .header {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        /* GUI */
        .gui {
            border: 2px solid black;
            padding: 1rem;
        }


        /* Form */
        .esp-form {
            display: flex;
            align-items: end;
            gap: 1rem;
        }

        .esp-form-input {
            display: flex;
            flex-direction: column;
        }

        /* Output */
        .output {
            border: 2px solid black;
            padding: 1rem;
        }

        .output-header {
            margin-top: 2rem;
            display: flex;
            align-items: center;
            gap: 2rem;
        }

        .response-list {
            border: 2px solid black;
            height: 50%;
            overflow-y: scroll;
            padding: 1rem;
            display: flex;
            flex-direction: column;
            gap: 1rem;
        }

        .response-row {]
            border: 2px solid black;
            padding-left: 1rem;
            padding-right: 1rem;
            font-size: 1rem;
            display: flex;
            align-items: center;
            justify-content: space-between;
        }

        .response-row span {
            font-weight: bold;
            margin-right: 1rem;
        }
    </style>


</head>

<body>
    <div class="app">

        <div class="header">
            <h1>ESP32 Web Server</h1>
            <span>An example of serving html content from an ESP32 web server</span>
        </div>

        <div class="gui">
            <div class="gui-header">
                <h2>Simple GUI</h2>
                <p>We can use a form to interact with the ESP32</p>
            </div>
            
            <form id="esp-form" class="esp-form">
                <div class="esp-form-input">
                    <label for="esp-form-text">Text</label>
                    <input type="text" id="esp-form-text" name="text" />
                </div>
                <div class="esp-form-input">
                    <label for="esp-form-value">Value</label>
                    <input type="number" min="0" max="65535" id="esp-form-value" name="value" />
                </div>
                <button type="submit">Submit</button>
            </form>
        </div>

        <div class="responses">
            <div class="responses-header">
                <h2>ESP Response</h2>
                <p>We can add the responses from the Esp here.</p>
                <button onclick="clearResponses()">Clear</button>
            </div>
            <div id="response-list" class="response-list">
            </div>
        </div>

    </div>

    <script>

        // window.addEventListener('load', refreshOutput);

        document
            .getElementById("esp-form")
            .addEventListener("submit", function (event) {
                event.preventDefault();
                const formData = new FormData(event.target);
                console.log("Form Data");
                for (let [key, value] of formData.entries()) {
                    console.log(key, ": ", value)
                }
                const text = formData.get("text");
                const value = formData.get("value");

                sendForm(text, value);
                event.target.reset();
            });

        // XHR Requests
        // Send the new keyfob data to the ESP32 server
        function sendForm(text, value) {
            const xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    const res = JSON.parse(this.responseText);
                    console.log(res);

                    addResponse(JSON.stringify(res));
                }
            };

            xhr.open("POST", `/esp-form?text=${text}&value=${value}`, true);
            xhr.send();
        }

        // Add responses to the output div.
        function addResponse(response) {

            const responseList = document.getElementById("response-list");

            // Add responses to our list in the form of divs
            const row = document.createElement("div");
            row.classList = "response-row";

            // We can style this div a little to make it look like a terminal.
            // We can also use some JS to add things like timestamps.

            const now = new Date(Date.now());
            const hours = now.getHours();
            const minutes = now.getMinutes();
            const seconds = now.getSeconds();
            const timestamp = hours + ":" + minutes + ":" + seconds;

            const span = document.createElement("span");
            span.textContent = timestamp + " >> ";

            // Add the response
            const p = document.createElement("p");
            p.appendChild(span);
            p.append(response)

            // Add all to the list
            row.appendChild(p);
            responseList.appendChild(row);

        }

        // Clear the responses from the div.
        function clearResponses() {

            const responseList = document.getElementById("response-list");
            // Make sure to first clear the list before adding anything.
            while (responseList.firstChild) {
                responseList.removeChild(responseList.firstChild);
            }

        }

    </script>

</body>

</html>
)ESPHTML";