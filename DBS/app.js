/*List TODO:
check if cus is connected 
logic to send via http the change of modality and slider value change
logic to update values of slider and change in modality received by the cus 
basta,credo
*/


// --- Stato Locale della Dashboard ---
let currentMode = 'AUTOMATIC'; // 'AUTOMATIC', 'MANUAL', 'UNCONNECTED', 'NOT_AVAILABLE'
let valveValue = 50;
let waterLevelHistory = [15, 18, 22, 25, 30, 35, 32, 28, 30, 34]; // Ultime N misurazioni debug

// --- Riferimenti DOM ---
const modeSwitchBtn = document.getElementById('modeSwitchBtn');
const statusBadge = document.querySelector('.status-badge');
const modeText = document.querySelector('.actions p');
const valveSlider = document.getElementById('valveSlider');
const percentageText = document.querySelector('.percentage');
const graphContainer = document.querySelector('.graph-placeholder');


// init Graph
graphContainer.innerHTML = '<canvas id="waterChart" style="width:100%; height:100%;"></canvas>';

const ctx = document.getElementById('waterChart').getContext('2d');
const waterChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: Array.from({ length: 10 }, (_, i) => `t-${9 - i}s`),
        datasets: [{
            label: 'Livello Acqua nella tank (cm)',
            data: waterLevelHistory,
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
                max: 100,
                title: { display: true, text: 'Livello (cm)' }
            }
        }
    }
});

// modality change button
modeSwitchBtn.addEventListener('click', () => {
    const newMode = currentMode === 'AUTOMATIC' ? 'MANUAL' : 'AUTOMATIC';
    updateSystemMode(newMode);
});

function updateSystemMode(mode) {
    currentMode = mode;

    if (mode === 'MANUAL') {
        statusBadge.textContent = 'MANUALE';
        statusBadge.className = 'status-badge manual';
        modeSwitchBtn.textContent = 'Passa ad AUTOMATICA';
        valveSlider.disabled = false; // enable dello slider
    } else if (mode === 'AUTOMATIC') {
        statusBadge.textContent = 'AUTOMATICA';
        statusBadge.className = 'status-badge automatic';
        modeSwitchBtn.textContent = 'Passa a MANUALE';
        valveSlider.disabled = true; // disble  lo slider
    }

    //TODO: aggiungi logica di invio modalita al cus
}

// real time value change della perchentuale al cambio dello slider
valveSlider.addEventListener('input', (e) => {
    if (currentMode === 'MANUAL') {
        valveValue = e.target.value;
        percentageText.textContent = `${valveValue}%`;
    }
});

//Update Graph
// Ogni 5 secondi simula la ricezione di nuove misurazioni
setInterval(() => {
    // 1. Simula una nuova lettura del livello dell'acqua
    const lastValue = waterLevelHistory[waterLevelHistory.length - 1];
    const delta = Math.floor(Math.random() * 9) - 4; 
    const newLevel = Math.max(5, Math.min(95, lastValue + delta));

    // Aggiorna la serie di dati nel grafico
    waterLevelHistory.shift();
    waterLevelHistory.push(newLevel);
    waterChart.data.datasets[0].data = waterLevelHistory;
    waterChart.update();
}, 5000);