--heading socat - das Schweizer Taschenmesser f�rs Netz
                          ��˿.                           
                          ��۲��.                         
                          -��۲p�p                        
                           -��۲�M�.                      
                            -��۲���,                     
                             -�۲۲ײ�                    
             �ܲ���ֿ.         M۲۲۲�                   
             -�۲�۲����,      ��۲۲���                  
               �ײ���ײ����,     Բ۲۲��.                
                �ӥ�ײ��ײײ˷,�  Ĳ�۲۲�,               
                   -�Mײ�ײײ����_ -�۲۲۲,              
                      ��M�ײ�ײײ�׷;M�۲۲�p             
                         ��Nײ�ײײײײ��۲��p            
                            �ϥ���ײ��ײ�ܲ�۲p           
                                �M�����ײ�������          
   .�,___��   ���:����:� -��:�:�:._?��M�ײ۲�۲�p         
 ��������������������������������yay�¿�ڿ_ZZ�m#qɿ,      
_��h����ڿ��=�=�||++||+|����{""���!��??��?��?�����N�/     
j��N�������������������׷��������׷�������������ܷ/)k     
�ײ��ײ������ײײ�ײ���Q�ײ˲ײײ�ײ�ײܲ�ܲײܲ�˲,�     
)ײ˲�����ײ�ײ�ײ�������ܲܲܲ�ײ�����߲�ײ�����߲x�,    
 ��ײײ�Ҳ�ײ�ײ��ײ�˲�Q���ײ�ײ�ײ�����ײ�ײ�ײ��@� 
   �������@T���?@Q�����""""���������������M�MM�MӾ��
      �߲��׷�,-Q�Ӳ��,                             
        ��ײ����Wp,�ײ׿.                              
           -�ײ���׷.{ܲ�,                                
              -�ײ�׿�ײץ,                               
                ��߲�۲������.                            
                    �߲�������˿..                        
                      -߲�����۲����                      
                        )�۲۲�۲۲ܷ�                    
                          4��۲� ���۲/                   
                           ĥ@�۲  �߲�/                  
                            ��Qײ�   ��'                  
                              ��߲�                       
--newpage
--heading socat - Multipurpose relay

* netcat++
  * netcat: Von Netz nach stdout / Von stdin zum Netz
  * socat: in und out (nahezu) beliebig konfigurierbar
  * Bidirektionales Relay zwischen zwei Bytestr�men

---
* Client-Connect an TCP-Port:
  * netcat ulm.ccc.de 80
  * socat - TCP4:ulm.ccc.de:80

---
* Mini-Server an Port binden:
  * netcat -l -p 1234
  * socat TCP4-LISTEN:1234,reuseaddr -

---
* Provisorische remote shell:
  * netcat -l -p 1234 -e /bin/bash
  * socat TCP4-LISTEN:1234,reuseaddr EXEC:/bin/bash,stderr
--newpage
--heading Und weiter...?

* "Semiprofessioneller" Server mit forking:
  socat TCP4-LISTEN:1234,reuseaddr,fork \
        EXEC:/bin/bash,stderr

---
* Zu unsicher? Na, dann mal nur an localhost binden:
  socat TCP4-LISTEN:1234,reuseaddr,fork,bind=localhost \
        EXEC:/bin/bash,stderr

---
* Zu restriktiv? Nur ein paar IPs zulassen...
  socat TCP4-LISTEN:1234,reuseaddr,fork,range=192.168.0.0/16 \
        EXEC:/bin/bash,stderr

---
* ...oder gleich als Dummuser in ein chroot-Jail:
  socat TCP4-LISTEN:1234,reuseaddr,fork,chroot=/tmp/sandbox,su=dummy \
        EXEC:/bin/bash,stderr
--newpage
--heading Parameteraufbau

* Adre�typ
  * Clients wie z.B. TCP4, IP4, UDP4 (gibt's alles auch f�r IPv6), OPENSSL
  * Server wie z.B. TCP4-LISTEN, IP4 (Raw-Socket), UDP4-LISTEN, OPENSSL-LISTEN
  * Socks-Server und Proxies: PROXY, SOCKS4
  * Standard-Streams: STDIN, STDOUT, STDERR, PTY
  * Verbesserte Eingabe mit History: READLINE
  * IPC: UNIX-CONNECT, UNIX-LISTEN, PIPE
  * Datei-IO: FILE
  * Programmaufrufe: EXEC, SYSTEM

* Zwingende Parameter
  * Mit Doppelpunkt getrennt

* Optionale Parameter
  * Durch Kommas getrennt

* man socat!
--newpage
--heading Mehr oder minder abstruse Anwendungsbeispiele

* "Mal kurz" ein Relay
  socat TCP4-LISTEN:1234,reuseaddr,fork \
        TCP4:destination.host:2345

---
* ...mit Debug-Ausgabe...
  * ASCII -v, HEX -x... oder beides:
--beginshelloutput
  $ socat -v -x TCP4-LISTEN:1234,reuseaddr TCP4:dest.host:2345
  >  48 65 6c 6c 6f 20 77 6f 72 6c 64 21 0a           Hello world!.
  --
  <  54 68 69 73 20 69 73 20 61 20 74 65 73 74 20 72  This is a test r
  <  65 70 6c 79 2e 2e 2e 20 54 68 69 73 20 69 73 20  eply... This is
  <  61 20 74 65 73 74 20 72 65 70 6c 79 2e 2e 2e 0a  a test reply....
  --
--endshelloutput

---
* "H�ndisches" scp
  Aus OpenSSL: /etc/ssl/misc/CA.sh -newcert
  socat -u OPENSSL-LISTEN:2345,reuseaddr, \
        cert=newcert.pem,key=newkey.pem,verify=false \
	FILE:/tmp/destfile
  socat -u FILE:/tmp/srcfile \
        OPENSSL:remote.host:2345,cafile=newcert.pem

---
* Primitiver Log-Daemon
  socat -u TCP4-LISTEN:3334,reuseaddr,fork \
           OPEN:/tmp/primitive.log,creat,append

---
* TCP gender changer 
  * Relay, wenn beide Parteien hinter NAT-Router (o.�.)
  socat TCP4-LISTEN:1234 TCP-LISTEN:2345
