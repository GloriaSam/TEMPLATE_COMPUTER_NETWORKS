
// Sammaritani Gloria 000072439
// TCP_Stream_Server.java

import java.io.*;
import java.net.*;

public class TCP_Stream_Server {

	private static final int PORT = 54321;

	public static void main(String[] args) {
		int port = -1;

		// controllo argomenti
		if (args.length == 0) {
			port = PORT;
			System.out.println("porta: " + PORT);
		} else if (args.length == 1) {
			try {
				port = Integer.parseInt(args[0]);
				if (port < 1024 || port > 65535) {
					System.out.println("Usage: java TCP_Stream_Server [serverPort>1024]");
					System.out.println("Errore: 1024<serverPort<65535.");
					System.exit(1);
				}
				System.out.println("porta: " + port);
			} catch (NumberFormatException e) {
				System.out.println("Usage: java TCP_Stream_Server [serverPort>1024]");
				System.out.println("serverPort deve essere un'intero");
				System.exit(1);
			}
		} else {
			System.out.println("Usage: java TCP_Stream_Server [serverPort>1024]");
			System.exit(1);
		}

		// dichiarazione e creazione socket
		ServerSocket serverSocket = null;
		Socket clientSocket = null;

		try {
			System.out.println("[TCP_Stream_Server]: Avvio");
			serverSocket = new ServerSocket(port);
			serverSocket.setReuseAddress(true);
			System.out.println("[TCP_Stream_Server]: Creata server socket " + serverSocket);

		} catch (Exception e) {
			System.out.println("Problemi durante la creazione della server socket: ");
			e.printStackTrace();
			System.out.println("[TCP_Stream_Server]: Termino");
			System.exit(1);
		}

		// TODO dichiarazione struttura e inizializzazione

		try {
			while (true) {
				System.out.println("[TCP_Stream_Server]: In attesa di richieste");
				try {
					clientSocket = serverSocket.accept();
					clientSocket.setSoTimeout(30000);
					System.out.println("[TCP_Stream_Server]: Connessione accettata " + clientSocket);

				} catch (Exception e) {
					System.out.println("Problemi durante l'accettazione della connessione: ");
					e.printStackTrace();
					continue;
				}
				// Creazione e avvio del thread
				try {
					new ServerThread(clientSocket).start();
				} catch (Exception e) {
					System.out.println("Problemi durante l'avvio del thread: ");
					e.printStackTrace();
					continue;
				}
			}
		} catch (Exception e) {
			System.out.println("Errore: ");
			e.printStackTrace();
			System.out.println("[TCP_Stream_Server]: Termino");
			System.exit(1);
		}
	}
}

class ServerThread extends Thread {

	private Socket clientSocket = null;
	// TODO struttura da aggiungere qui negli arg e nel costruttore

	// costruttore aggiungi anche la struttura
	public ServerThread(Socket clientSocket) {
		this.clientSocket = clientSocket;
	}

	// run
	public void run() {

		// TODO VARIABILI PER LA LOGICA DEL PROGRAMMA
		DataInputStream inSock;
		DataOutputStream outSock;
		// ObjectOutputStream outSockOb = null;
		String richiesta = null;
		String risposta = null;
		String command = null;

		try {
			inSock = new DataInputStream(clientSocket.getInputStream());
			outSock = new DataOutputStream(clientSocket.getOutputStream());
			// outSockOb = new ObjectOutputStream();
			// ObjectOutputStream(clientSocket.getOutputStream());
		} catch (IOException e) {
			System.out.println("Problemi durante la creazione degli stream: ");
			e.printStackTrace();
			return;
		}

		System.out.println("[ServerThread]: Avvio");
		try {
			while (!clientSocket.isInputShutdown()) { 
				// recezione richieste da client
				command = inSock.readUTF();

				if (command.equals("")) {
					// TODO RICEVO RICHIESTA ED ELABORO ESITO
					System.out.println("Ricevuta richiesta di ");
					richiesta = inSock.readUTF();

					// TODO INVIO ESITO (esempio stringa)
					System.out.println("Invio esito: " + esito);
					outSock.writeUTF(esito);

				} else if (command.equals("")) {
					// TODO RICEVO RICHIESTA ED ELABORO ESITO
					System.out.println("Ricevuta richiesta di ");
					richiesta = inSock.readUTF();

					// TODO INVIO ESITO (esempio stringa)
					System.out.println("Invio esito: " + esito);
					outSock.writeUTF(esito);

				} else {
					System.out.println("Comando errato");
				}
			} // chiusura while
		} catch (Exception e) {
			System.out.println("Errore: ");
			e.printStackTrace();
			System.out.println("[ServerThread]: Termino");
			return;
		}
		System.out.println("[ServerThread]: Termino");
	}
}
