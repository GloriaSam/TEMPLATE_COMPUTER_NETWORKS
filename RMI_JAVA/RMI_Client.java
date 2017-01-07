// Sammaritani Gloria 0000723439
// RMI_Client.java

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;


public class RMI_Client {

    // avvio del client RMI
    public static void main(String[] args) {
        final int REGISTRYPORT = 1099;
        String registryHost = null;
        String serviceName = "RMI_service_name"; //TODO
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
        String command = null;

        // controllo argomenti
        if(args.length != 1){
            System.out.println("Usage: RMI_Client serverAddress");
            System.exit(1);
        }
        registryHost = args[0];

        // connessione al servizio RMI remoto
        try{
            String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
            RMI_InterfaceFile serverRMI = (RMI_InterfaceFile) Naming.lookup(completeName);
            System.out.println("[RMI_Client]: Service \"" + serviceName + "\" connected");

            // TODO LOGICA CLIENT
            while(true){
                System.out.println("Opzioni:)\nEOF) terminare il programma");
                command = stdIn.readLine().trim();
                switch(command){
                    case ""://intereazione con utente per funzione
                            break;
                    default: System.out.println("L'opzione inserita non esiste");
                }
            }

        }	catch(Exception e){
            e.printStackTrace();
            System.exit(1);
        }
    }
}
