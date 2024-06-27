import os
import random

def generate_data(num_files, num_integers):
    # Create a directory for data if it does not exist
    data_dir = 'data'
    if not os.path.exists(data_dir):
        os.makedirs(data_dir)

    # Generate data files
    for i in range(num_files):
        # Create random integers
        data = [random.randint(0, 10000) for _ in range(num_integers)]
        
        # Write the data to a file
        file_path = os.path.join(data_dir, f'data_{num_integers}_{i+1}.txt')
        with open(file_path, 'w') as f:
            f.write('\n'.join(map(str, data)))

        print(f'Data written to {file_path}')

# Configuration
num_files = 10  # Number of data files to generate
num_integers = 100000  # Number of integers per file

# Generate data
generate_data(num_files, num_integers)
