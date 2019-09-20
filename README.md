# mirquest2-api

This is an Django REST API built for the miRQuest 2 project, a master's degree research project to integrate micro RNA prediction tools into one place. By now, there are 2 preditions tools being integrated:

- [Mirinho](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4448272/)
- [miRBoost](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4408786/)

## How to install 

- Clone the repo
- [Create a virtualEnv](https://gist.github.com/Geoyi/d9fab4f609e9f75941946be45000632b)
- Install the requirements
- Build the *Mirinho* and *miRBoost* source code
  - Go to the tool folder and run the `make` command
- Migrate the database - `python manage.py migrate`
- Load the tool module data - `python manager.py loaddata tools/fixtures/tool_data.json`
- Run the server - `python manage.py runserver`


### Observations
- The miRBoost is using an updated `libsvm` 
- Some environments that have some different versions of gcc compiler may have some trouble compiling the prediciton tools source code 
