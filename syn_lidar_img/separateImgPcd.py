import os
import shutil

# Set the paths
source_folder = '/home/janeqin/code/lidar_img_syn_lmx/syn_lidar_img/lidar_img'
jpg_destination_folder = '/home/janeqin/code/lidar_img_syn_lmx/syn_lidar_img/image'
pcd_destination_folder = '/home/janeqin/code/lidar_img_syn_lmx/syn_lidar_img/lidar'

# Create destination folders if they don't exist
os.makedirs(jpg_destination_folder, exist_ok=True)
os.makedirs(pcd_destination_folder, exist_ok=True)

# Iterate over all files in the source folder
for filename in os.listdir(source_folder):
    file_path = os.path.join(source_folder, filename)
    
    # Check if the file is a .jpg or .pcd file
    if filename.endswith('.jpg'):
        shutil.move(file_path, os.path.join(jpg_destination_folder, filename))
    elif filename.endswith('.pcd'):
        shutil.move(file_path, os.path.join(pcd_destination_folder, filename))

print("Files separated and moved successfully!")
