const API_URL = 'http://localhost:8080/api/data';

// init Stato Locale
let currentMode = 'AUTOMATIC';
let valveValue = 50;
let isDraggingSlider = false;

// DOM
const modeSwitchBtn = document.getElementById('modeSwitchBtn');
const badgeConStatus = document.querySelector('.dot');
const conStatus = document.querySelectorAll('span')[1];
const statusBadge = document.querySelector('.status-badge');
const valveSlider = document.getElementById('valveSlider');
const percentageText = document.querySelector('.percentage');
const graphContainer = document.querySelector('.graph-placeholder');

// init Graph
graphContainer.innerHTML = '<canvas id="waterChart" style="width:100%; height:100%;"></canvas>';
const ctx = document.getElementById('waterChart').getContext('2d');
const waterChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: Array.from({ length: 30 }, (_, i) => `t-${30 - i}s`),
        datasets: [{
            label: 'Livello Acqua nella tank (mm)',
            data: [],
            borderColor: '#2563eb',
            backgroundColor: 'rgba(37, 99, 235, 0.1)',
            borderWidth: 2,
            fill: true,
            tension: 0.3
        }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
            y: {
                beginAtZero: true,
                max: 1000,
                title: { display: true, text: 'Livello (mm)' }
            }
        }
    }
});

//#region Chiamate HTTP

// GET: Recupera stato attuale e storico dal CUS
async function fetchSystemStatus() {
    try {
        const response = await fetch(API_URL);
        if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`);

        const data = await response.json(); // { mode, opening, history }

        setConnectionStatus(true);

        updateSystemUI(data.mode, data.opening);

        // Aggiorna il grafico
        if (Array.isArray(data.history)) {
            waterChart.data.datasets[0].data = data.history;
            waterChart.update();
        }
    } catch (error) {
        console.error("Errore nel recupero dati dal CUS:", error);
        setConnectionStatus(false);
        setCusUnreachableUI();
    }
}

// POST: Invia la nuova modalità e/o la percentuale della valvola
async function sendSystemStatus(mode, opening) {
    try {
        const response = await fetch(API_URL, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                mode: mode,
                opening: parseInt(opening, 10)
            })
        });

        if (response.status === 409) {
            alert("Impossibile modificare: il CUS è disconnesso (UNCONNECTED).");
            fetchSystemStatus();
        } else if (!response.ok) {
            console.error("Errore nell'invio dello stato:", response.status);
        }
    } catch (error) {
        console.error("Errore di rete durante la sincronizzazione:", error);
    }
}
//#endregion

//#region UI

function updateSystemUI(mode, opening) {
    currentMode = mode;

    resetModeButtonStyle();// toglie i disbled

    if (mode === 'UNCONNECTED') {//TMS disconnected not CUS
        statusBadge.textContent = 'UNCONNECTED';
        statusBadge.className = 'status-badge unconnected';
        valveSlider.disabled = true;
        modeSwitchBtn.disabled = true;
        modeSwitchBtn.style.backgroundColor = "lightgray";
        modeSwitchBtn.style.color = "gray";
        return;
    }

    // impedisco il refresh del valore se lo sto cambiando in locale
    if (!isDraggingSlider) {
        valveValue = opening;
        valveSlider.value = valveValue;
        percentageText.textContent = `${valveValue}%`;
    }

    if (mode === 'MANUAL') {
        statusBadge.textContent = 'MANUALE';
        statusBadge.className = 'status-badge manual';
        modeSwitchBtn.textContent = 'Passa ad AUTOMATICA';
        modeSwitchBtn.disabled = false;
        valveSlider.disabled = false;
    } else if (mode === 'AUTOMATIC') {
        statusBadge.textContent = 'AUTOMATICA';
        statusBadge.className = 'status-badge automatic';
        modeSwitchBtn.textContent = 'Passa a MANUALE';
        modeSwitchBtn.disabled = false;
        valveSlider.disabled = true;
    }
}

// only for cus
function setConnectionStatus(connected) {
    if (connected) {
        badgeConStatus.classList.remove('offline');
        badgeConStatus.classList.add('online');
        conStatus.textContent = "CUS Connesso";
    } else {
        badgeConStatus.classList.remove('online');
        badgeConStatus.classList.add('offline');
        conStatus.textContent = "CUS Disconnesso";
    }
}

function setCusUnreachableUI() {
    statusBadge.textContent = "UNAVAILABLE";
    statusBadge.className = 'status-badge not-available';
    valveSlider.disabled = true;
    modeSwitchBtn.disabled = true;
    modeSwitchBtn.style.backgroundColor = "lightgray";
    modeSwitchBtn.style.color = "gray";
}

function resetModeButtonStyle() {
    modeSwitchBtn.style.backgroundColor = "";
    modeSwitchBtn.style.color = "";
}

//#endregion 

//#region Events

//cambio modalità
modeSwitchBtn.addEventListener('click', () => {
    if (currentMode === 'UNCONNECTED') return;

    const newMode = currentMode === 'AUTOMATIC' ? 'MANUAL' : 'AUTOMATIC';

    sendSystemStatus(newMode, valveValue);
});

// Segna l'inizio del trascinamento
valveSlider.addEventListener('pointerdown', () => {
    isDraggingSlider = true;
});

// update grafico valore slider
valveSlider.addEventListener('input', (e) => {
    if (currentMode === 'MANUAL') {
        valveValue = e.target.value;
        percentageText.textContent = `${valveValue}%`;
    }
});

// Invio new valore slider
valveSlider.addEventListener('change', (e) => {
    isDraggingSlider = false; // slider rilasciato
    if (currentMode === 'MANUAL') {
        sendSystemStatus(currentMode, e.target.value);
    }
});

//#endregion

// Primo recupero immediato
fetchSystemStatus();

// polling ogni secondo
setInterval(fetchSystemStatus, 1000);