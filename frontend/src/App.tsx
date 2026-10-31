import { useState } from "react";
import {
  Activity,
  ArrowDown,
  ArrowUp,
  Server,
  Zap,
  Cloud,
  Wifi,
  WifiOff,
} from "lucide-react";
import "./App.css";

type LoadState = "LOW" | "HIGH";

function App() {
  const [load, setLoad] = useState<LoadState>("LOW");
  const [connected, setConnected] = useState(false);
  const [lastUpdated, setLastUpdated] = useState("--");
  const [message, setMessage] = useState("");


  // Increase load
  const handleIncreaseLoad = async () => {
    setLoad("HIGH");
    setMessage("Workload changed to HIGH.");

    try {
      const response = await fetch("/api/load/increase", {
        method: "POST",
      });

      if (!response.ok) {
        throw new Error(`HTTP Error: ${response.status}`);
      }

      setConnected(true);
      setLastUpdated(new Date().toLocaleTimeString());
      setMessage("");

      console.log("Request sent:", {
        method: "POST",
        path: "/api/load/increase",
      });

    } catch (error) {
      console.error("Failed to send workload:", error);

      setConnected(false);
      setMessage("Unable to communicate with Load Balancer.");

    }
  };

  // Decrease load
  const handleDecreaseLoad = async () => {
    setLoad("LOW");
    setMessage("Workload changed to LOW.");

    try {
      const response = await fetch("/api/load/decrease", {
        method: "POST",
      });

      if (!response.ok) {
        throw new Error(`HTTP Error: ${response.status}`);
      }

      setConnected(true);
      setLastUpdated(new Date().toLocaleTimeString());
      setMessage("");

      console.log("Request sent:", {
        method: "POST",
        path: "/api/load/decrease",
      });

    } catch (error) {
      console.error("Failed to send workload:", error);

      setConnected(false);
      setMessage("Unable to communicate with Load Balancer.");

    }
  };

  return (
    <div className="app-container">

      {/* Sidebar */}
      <aside className="sidebar">
        <div className="logo">
          <Cloud size={30} />
          <span>AutoScaler</span>
        </div>

        <nav className="sidebar-nav">
          <div className="nav-item active">
            <Activity size={20} />
            <span>Dashboard</span>
          </div>

          <div className="nav-item">
            <Server size={20} />
            <span>Infrastructure</span>
          </div>
        </nav>

        <div className="sidebar-bottom">
          <div className="connection-indicator">
            <span
              className={`status-dot ${connected ? "online" : "offline"
                }`}
            />

            <span>
              {connected
                ? "Load Balancer Online"
                : "Load Balancer Offline"}
            </span>
          </div>
        </div>
      </aside>

      {/* Main Content */}
      <main className="main-content">

        {/* Header */}
        <header className="top-header">
          <div>
            <h1>Cloud Auto-Scaler</h1>
            <p>Monitor and manage your workload</p>
          </div>

          <div className="header-right">
            <div className="connection-status">
              {connected ? (
                <Wifi size={18} />
              ) : (
                <WifiOff size={18} />
              )}

              <span>
                {connected ? "Connected" : "Disconnected"}
              </span>
            </div>
          </div>
        </header>

        {/* Overview Cards */}
        <section className="overview-grid">

          {/* Load Balancer Status */}
          <div className="dashboard-card">
            <div className="card-heading">
              <span>Load Balancer Status</span>
              <Server size={22} />
            </div>

            <h2
              className={
                connected ? "success-text" : "danger-text"
              }
            >
              {connected ? "Online" : "Offline"}
            </h2>

            <p>
              {connected
                ? "Load Balancer is responding"
                : "Unable to reach Load Balancer"}
            </p>
          </div>

          {/* Current Load */}
          <div className="dashboard-card">
            <div className="card-heading">
              <span>Current Load</span>
              <Activity size={22} />
            </div>

            <h2
              className={
                load === "HIGH"
                  ? "warning-text"
                  : "success-text"
              }
            >
              {load}
            </h2>

            <p>
              {load === "HIGH"
                ? "High workload mode is active"
                : "Low workload mode is active"}
            </p>
          </div>

          {/* Workload Generator */}
          <div className="dashboard-card">
            <div className="card-heading">
              <span>Workload Generator</span>
              <Zap size={22} />
            </div>

            <h2>Active</h2>

            <p>
              Sending workload to the Load Balancer
            </p>
          </div>

        </section>

        {/* Workload Control */}
        <section className="control-section">

          <div className="section-heading">
            <div>
              <h2>Workload Control</h2>
              <p>
                Change the workload generated by the client
              </p>
            </div>

            <span className="live-badge">
              <span className="live-dot" />
              LIVE STATUS
            </span>
          </div>

          <div className="control-card">

            <div className="current-load">
              <div className="load-icon">
                <Activity size={30} />
              </div>

              <div>
                <p>Current Workload State</p>
                <h2>{load} LOAD</h2>
              </div>
            </div>

            <div className="control-description">
              {load === "LOW"
                ? "Generating requests every 1000 ms with values between 10 and 15."
                : "Generating requests every 100 ms with values between 20 and 30."}
            </div>

            <div className="control-buttons">

              <button
                className="increase-button"
                onClick={handleIncreaseLoad}
                disabled={load === "HIGH"}
              >
                <ArrowUp size={19} />
                Increase Load
              </button>

              <button
                className="decrease-button"
                onClick={handleDecreaseLoad}
                disabled={load === "LOW"}
              >
                <ArrowDown size={19} />
                Decrease Load
              </button>

            </div>

            {message && (
              <p className="feedback-message">
                {message}
              </p>
            )}

          </div>
        </section>

        {/* System Information */}
        <section className="system-section">

          <div className="section-heading">
            <div>
              <h2>System Information</h2>
              <p>
                Current workload generation details
              </p>
            </div>
          </div>

          <div className="system-card">

            <div className="system-row">
              <span>Load Balancer API</span>
              <strong>127.0.0.1:8000/work</strong>
            </div>

            <div className="system-row">
              <span>Backend Connection</span>
              <strong
                className={
                  connected ? "success-text" : "danger-text"
                }
              >
                {connected ? "Connected" : "Disconnected"}
              </strong>
            </div>

            <div className="system-row">
              <span>Current Load State</span>
              <strong>{load}</strong>
            </div>

            <div className="system-row">
              <span>Last Request</span>
              <strong>{lastUpdated}</strong>
            </div>

          </div>
        </section>

        {/* Footer */}
        <footer className="footer">
          <p>Cloud Auto-Scaler Dashboard</p>
          <p>React + TypeScript | C++ Load Balancer</p>
        </footer>

      </main>
    </div>
  );
}

export default App;
