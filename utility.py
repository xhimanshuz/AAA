import wget
import requests
import json
import subprocess

GITHUB_API_URL = "https://api.github.com/repos"

class Update():
    def __init__(self):
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
        with open("status.json", "r") as f:
            self.local_json = json.load(f)
            print("[!] Local Json: ", self.local_json)

    def getJson(self):
        '''
        Downloading Json from server
        '''
        print(f"Getting Json From server, Url: {self.status_json_url}")
        resp = requests.get(self.status_json_url)
        if resp.status_code == 200:
            self.json = resp.json()
        print(f"Response: {self.json}")
    
    def downloadFile(self):
        file_url = self.json['file_url']
        try:
            filename = wget.download(file_url)
            print(f"File Success fully downloaded at: {self.save_location+self.json['file_name']} ")
        except Exception as e:
            print(f"Error in Downloading {file_url}, resp code = {filename}")
            print(f"Exception Occured: ", e)
            return False
        return self.save_location+self.json['file_name']

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
        pass

    def run(self):
        self.getJson()
        self.openJson()
        print("Downloading Upate File", )
        if not self.versionCheck():
            return 
        file = self.downloadFile()
        self.runArchive(file)
# if __file__ == __name__:
u = Update()
u.run()