import os
import wget
import requests
import json
import subprocess
import sys
from sys import exit

GITHUB_API_URL = "https://api.github.com/repos"

class Update():
    def __init__(self, _password):
        self.password = _password
        self.repo_url = "/xhimanshuz/AAA"
        self.status_json_url = 'https://raw.githubusercontent.com/xhimanshuz/AAA/master/status.json'
        self.local_json = {}
        self.json = {}
        self.github_repo_url = '"https://api.github.com/repos/xhimanshuz/AAA/contents/'
        self.save_location = ""

    def openJson(self):
        '''
        Load Local Json
        '''
        print(f"Loading Local Json: ")
        try:
            with open("status.json", "r") as f:
                self.local_json = json.load(f)
                print("[!] Local Json: ", self.local_json)
        except Exception as e:
            self.local_json['version'] = '0'
            print("Exception while opening Json.")

    def getJson(self):
        '''
        Downloading Json from server
        '''
        print(f"Getting Json From server, Url: {self.status_json_url}")
        resp = requests.get(self.status_json_url)
        if resp.status_code == 200:
            self.json = resp.json()
        print(f"Response: {self.json}")

    def writeJson(self):
        print("Updating Exisiting Json")
        with open("status.json", 'w') as f:
            json.dump(self.json, f)
        print("Successfully Download Json")
    
    def downloadFile(self):
        file_url = self.json['file_url']
        try:
            if os.path.exists(self.json['file_name']):
                os.remove(self.json['file_name'])
            filename = wget.download(file_url, self.json['file_name'])
            print(f"File Success fully downloaded at: {self.save_location+filename} ")
        except Exception as e:
            print(f"Error in Downloading {file_url}, resp code = {filename}")
            print(f"Exception Occured: ", e)
            return False
        return self.save_location+filename

    def versionCheck(self):
        print("[!] Checking Version.")
        local_version = self.local_json['version']
        server_version = self.json['version']
        print(f"Local version: {local_version}, Server Version: {server_version}")
        if local_version == server_version:
            print("AAA Application is updated")
            return False
        local_version = int(local_version.replace(".",""))
        server_version = int(server_version.replace(".",""))
        if server_version > local_version:
            print("Local version is outdate, Downloading new one")
            return True

    def runArchive(self, fileName):
        print(f"Running Archive: {fileName}")
        subprocess.run('taskkill /IM "AAA.exe" /F')
        resp = subprocess.run(f"{fileName} -y -p{self.password}")
        if not resp.returncode:
            print(f"Success Fully Updated, {resp.returncode}")
        else:
            print(f"Error in Installing File, {resp.returncode}")
            input("Press Enter to Continue...")
            exit(-2)

    def removeDownloadedFile(self, file):
        print(f"Cleaning Archive File... {file}")
        if os.path.exists(file):
            os.remove(file)

    def run(self):
        self.getJson()
        self.openJson()
        print("Downloading Upate File", )
        if not self.versionCheck():
            input("Press Enter to Continue...")
            exit(0)
        file = self.downloadFile()
        if not file:
            input("Press Enter to Continue...")
            exit(-1)
        input("Now, File downloaded, Do you want to install, \nIt will close the application? Press Enter to continue.")
        self.runArchive(file)
        self.writeJson()
        #self.removeDownloadedFile(file)
    
def main():
    try:
        password = sys.argv[1]
        # print("Password: ", password)
        u = Update(password)
        u.run()
        input("Press Enter to Continue...")
        exit(0)
    except Exception as e:
        print(f"Exception occured, {str(e)}")
        input("Press Enter to Continue...")
        exit(1)

main()