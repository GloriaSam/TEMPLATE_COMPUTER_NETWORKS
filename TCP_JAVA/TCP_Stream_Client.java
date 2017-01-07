
// Sammaritani Gloria 000072439
// TCP_Stream_Client.java

import java.net.*;
import java.io.*;

public class TCP_Stream_Client {

	public static void main(String[] args) throws IOException {

		InetAddress addr = null;
		int port = -1;

		// controllo arogmenti
		try {
			if (args.length == 2) {
				addr = InetAddress.getByName(args[0]);
				port = Integer.parseInt(args[1]);
			} else {
				System.out.println("Usage: java TCP_Stream_Client serverIP serverPort");
				System.exit(1);
			}
		} catch (UnknownHostException e) {
			System.out.println("Problemi durante la determinazione dell'endpoint del server : ");
			e.printStackTrace();
			System.exit(1);
		} catch (NumberFormatException e) {
			System.out.println("Usage: java TCP_Stream_Client serverIP serverPort");
			System.out.println("serverPort deve essere un intero");
			System.exit(1);
		}

		// TODO VARIABILI PER LA LOGICA DEL PROGRAMMA
		String richiesta = null;
		String esito = null;

		// dichiarazione e creazione socket
		Socket socket = null;
		DataInputStream inSock = null;
		DataOutputStream outSock = null;
		// ObjectInputStream inSockOb = null
		BufferedReader in = null;
		try {
			socket = new Socket(addr, port);
			socket.setSoTimeout(30000);
			System.out.println("Creata la socket: " + socket);
		} catch (Exception e) {
			System.out.println("Problemi durante la creazione della socket: ");
			e.printStackTrace();
			System.exit(1);
		}
		try {
			inSock = new DataInputStream(socket.getInputStream());
			outSock = new DataOutputStream(socket.getOutputStream());
			// inSockOb = new ObjectInputStream(socket.getInputStream());
		} catch (Exception e) {
			System.out.println("Problemi durante la creazione degli stream: ");
			e.printStackTrace();
			System.exit(1);
		}
		//TODO inzializza variabili
		in = new BufferedReader(new InputStreamReader(System.in));
		String command = null;
		
		try {
			System.out.println("Inserisci: \n-) \n-) \nEOF per terminare: ");
			while ((command = in.readLine().trim()) != null) {

				outSock.writeUTF(command);

				if (command.equals("")) {
					// TODO INTERAZIONE CON L'UTENTE E INVIO RICHIESTA
					System.out.println("");
					outSock.writeUTF(richiesta);

					// TODO RICEZIONE ESITO (esempio stringa)
					esito = inSock.readUTF();
					System.out.println("Esito: " + esito);

				} else if (command.equals("")) {
					// TODO INTERAZIONE CON L'UTENTE E INVIO RICHIESTA
					System.out.println("");
					outSock.writeUTF(richiesta);

					// TODO RICEZIONE ESITO (esempio stringa)
					esito = inSock.readUTF();
					System.out.println("Esito: " + esito);

				} else {
					System.out.println("Comando non esistente");
				}

				System.out.println("Inserisci: \n-) \n-) \nEOF per terminare: ");
			} // chiudo while
		} catch (SocketTimeoutException ste) {
			System.out.println("Timeout scattato: ");
			ste.printStackTrace();
			socket.close();
			System.exit(1);
		} catch (IOException e) {
			System.out.println("Errore durante la ricezione dell'esito: ");
			e.printStackTrace();
			socket.close();
			System.exit(1);
		}
	}
}
