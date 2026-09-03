package cus.gui;

import cus.model.SystemMode;
import cus.model.SystemState;

import javax.swing.*;
import java.awt.*;

public class MainFrame extends JFrame {

    private final SystemState state;

    private JLabel modeValue;
    private JLabel levelValue;
    private JLabel valveValue;
    private JLabel lastSeenValue;

    public MainFrame(SystemState state) {
        super("Smart Tank Monitoring — CUS");
        this.state = state;

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(360, 270);
        setLocationRelativeTo(null);

        buildUi();

        new Timer(500, e -> refresh()).start();
        refresh();
    }

    private void buildUi() {
        JPanel root = new JPanel();
        root.setLayout(new BoxLayout(root, BoxLayout.Y_AXIS));
        root.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        JLabel title = new JLabel("Stato TMS");
        title.setFont(title.getFont().deriveFont(Font.PLAIN, 12f));
        title.setAlignmentX(Component.LEFT_ALIGNMENT);

        modeValue = new JLabel("--");
        modeValue.setFont(modeValue.getFont().deriveFont(Font.BOLD, 30f));
        modeValue.setAlignmentX(Component.LEFT_ALIGNMENT);

        root.add(title);
        root.add(modeValue);
        root.add(Box.createVerticalStrut(20));

        levelValue = addRow(root, "Livello (mm):");
        valveValue = addRow(root, "Apertura valvola:");
        lastSeenValue = addRow(root, "Ultimo dato ricevuto:");

        setContentPane(root);
    }

    private JLabel addRow(JPanel root, String label) {
        JPanel row = new JPanel(new BorderLayout());
        row.setAlignmentX(Component.LEFT_ALIGNMENT);
        row.setMaximumSize(new Dimension(Integer.MAX_VALUE, 26));

        JLabel labelComponent = new JLabel(label);
        JLabel valueComponent = new JLabel("--");
        valueComponent.setFont(valueComponent.getFont().deriveFont(Font.BOLD, 15f));

        row.add(labelComponent, BorderLayout.WEST);
        row.add(valueComponent, BorderLayout.EAST);
        root.add(row);

        return valueComponent;
    }

    private void refresh() {
        SystemMode mode = state.getMode();
        modeValue.setText(mode.name());
        modeValue.setForeground(colorForMode(mode));

        levelValue.setText(state.getLevel() + " mm");
        valveValue.setText(state.getValveOpen() + " %");
        lastSeenValue.setText(agoText(state.getLastTmsMessageAt()));
    }

    private Color colorForMode(SystemMode mode) {
        switch (mode) {
            case AUTOMATIC: return new Color(0x2e7d32);
            case MANUAL: return new Color(0xef6c00);
            default: return new Color(0xc62828);
        }
    }

    private String agoText(long timestampMillis) {
        if (timestampMillis == 0) return "mai";
        long secondsAgo = (System.currentTimeMillis() - timestampMillis) / 1000;
        return secondsAgo + "s fa";
    }
}