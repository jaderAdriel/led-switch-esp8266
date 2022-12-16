import { initializeApp } from 'firebase/app'
import { getDatabase, ref, set, onValue } from "firebase/database";

const firebaseConfig = {
    apiKey: "AIzaSyDwa6dcBGeQ5_Br26Ffxhc0fESnhsz-0Bc",
    authDomain: "leds-ea76c.firebaseapp.com",
    projectId: "leds-ea76c",
    storageBucket: "leds-ea76c.appspot.com",
    messagingSenderId: "928737867061",
    appId: "1:928737867061:web:592b69bfc10ae746d7887c",
    databaseURL: "https://leds-ea76c-default-rtdb.firebaseio.com/",
};

initializeApp(firebaseConfig);

const database = getDatabase();

function changeLedState(ledColor, state) {
    
    set(ref(database, 'led/' + ledColor), {
        light_on : state,
    });
}

window.addEventListener("load", (event) => {
    main()
});


function main() {

    const led_colors = ['red', 'green', 'blue'];
    const ledStateInputs = document.querySelectorAll("input[name='led__state']");

    ledStateInputs.forEach( (e) => {

        let inputColorName = e.id;
    
    
        for (let i = 0; i < led_colors.length ; i ++ ) {
    
            if (inputColorName.search(led_colors[i]) > -1) {
                
                let ledRef = ref(database, 'led/' + led_colors[i]);
                // Pega os valores que estao no banco de dados e adiciona no inputs
                onValue(ledRef, (light_on) => {
                    const data = light_on.val();
                    e.checked = data.light_on;
                });
            }
        }
    
        e.addEventListener('click', (event) => {
            
            let state = e.checked;
            
            for (let i = 0; i < led_colors.length ; i ++ ) {
                if (inputColorName.search(led_colors[i]) > -1) {
                    changeLedState(led_colors[i], state)
                    return
                }
            }
        });
    });
    
}




