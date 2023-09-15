port_number=$(cat data.config | grep PORT | sed)
kill $(lsof -t -i :PORT)