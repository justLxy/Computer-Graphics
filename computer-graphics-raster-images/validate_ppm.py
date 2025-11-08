#!/usr/bin/env python3
"""
PPM Validation Script
Compares generated PPM files against reference PPM files with error tolerance.
"""

import os
import sys
import numpy as np
from pathlib import Path

def parse_ppm(file_path):
    """Parse a PPM/PGM file and return header info and image data."""
    with open(file_path, 'r') as f:
        lines = f.readlines()
    
    # Parse header
    line_idx = 0
    
    # Skip comments and get magic number
    while lines[line_idx].startswith('#'):
        line_idx += 1
    magic = lines[line_idx].strip()
    line_idx += 1
    
    # Skip comments and get dimensions
    while lines[line_idx].startswith('#'):
        line_idx += 1
    width, height = map(int, lines[line_idx].strip().split())
    line_idx += 1
    
    # Skip comments and get max value
    while lines[line_idx].startswith('#'):
        line_idx += 1
    max_val = int(lines[line_idx].strip())
    line_idx += 1
    
    # Read image data
    image_data = []
    for line in lines[line_idx:]:
        if line.strip():  # Skip empty lines
            values = line.strip().split()
            image_data.extend([int(val) for val in values])
    
    # Reshape based on format
    if magic == 'P2':  # Grayscale
        image_array = np.array(image_data).reshape(height, width)
        channels = 1
    elif magic == 'P3':  # RGB
        image_array = np.array(image_data).reshape(height, width, 3)
        channels = 3
    else:
        raise ValueError(f"Unsupported format: {magic}")
    
    return {
        'magic': magic,
        'width': width,
        'height': height,
        'max_val': max_val,
        'image_data': image_array,
        'channels': channels
    }

def compare_ppm_files(generated_file, reference_file, tolerance=1):
    """Compare two PPM files with given tolerance."""
    
    print(f"Comparing:")
    print(f"  Generated: {generated_file}")
    print(f"  Reference: {reference_file}")
    print(f"  Tolerance: {tolerance}")
    print()
    
    try:
        # Parse both files
        gen_data = parse_ppm(generated_file)
        ref_data = parse_ppm(reference_file)
        
        # Check basic properties
        if gen_data['magic'] != ref_data['magic']:
            print(f"❌ Magic number mismatch: {gen_data['magic']} vs {ref_data['magic']}")
            return False
            
        if gen_data['width'] != ref_data['width'] or gen_data['height'] != ref_data['height']:
            print(f"❌ Dimension mismatch: {gen_data['width']}x{gen_data['height']} vs {ref_data['width']}x{ref_data['height']}")
            return False
            
        if gen_data['max_val'] != ref_data['max_val']:
            print(f"❌ Max value mismatch: {gen_data['max_val']} vs {ref_data['max_val']}")
            return False
        
        print(f"✅ Format: {gen_data['magic']} ({'RGB' if gen_data['channels'] == 3 else 'Grayscale'})")
        print(f"✅ Dimensions: {gen_data['width']}x{gen_data['height']}")
        print(f"✅ Max value: {gen_data['max_val']}")
        
        # Compare image data
        gen_img = gen_data['image_data']
        ref_img = ref_data['image_data']
        
        # Calculate differences
        diff = np.abs(gen_img.astype(int) - ref_img.astype(int))
        max_diff = np.max(diff)
        mean_diff = np.mean(diff)
        
        # Count values that exceed tolerance
        pixels_over_tolerance = np.sum(diff > tolerance)
        if gen_data['channels'] == 3:
            total_values = gen_img.shape[0] * gen_img.shape[1] * 3  # height * width * 3 channels
        else:
            total_values = gen_img.shape[0] * gen_img.shape[1]  # height * width
        
        print(f"📊 Image Comparison:")
        print(f"  Max difference: {max_diff}")
        print(f"  Mean difference: {mean_diff:.3f}")
        print(f"  Values over tolerance: {pixels_over_tolerance}/{total_values} ({100*pixels_over_tolerance/total_values:.3f}%)")
        
        # Check if within tolerance
        if max_diff <= tolerance:
            print(f"✅ PASS: All values within tolerance of {tolerance}")
            return True
        else:
            print(f"❌ FAIL: Some values exceed tolerance of {tolerance}")
            
            # Show some examples of differences
            over_tolerance_mask = diff > tolerance
            if np.any(over_tolerance_mask):
                print("\n🔍 Examples of differences > tolerance:")
                positions = np.where(over_tolerance_mask)
                for i in range(min(5, len(positions[0]))):  # Show up to 5 examples
                    if gen_data['channels'] == 3:  # RGB
                        row, col, channel = positions[0][i], positions[1][i], positions[2][i]
                        gen_val = gen_img[row, col, channel]
                        ref_val = ref_img[row, col, channel]
                        difference = diff[row, col, channel]
                        channel_name = ['R', 'G', 'B'][channel]
                        print(f"  Position ({row},{col}) {channel_name}: Generated={gen_val}, Reference={ref_val}, Diff={difference}")
                    else:  # Grayscale
                        row, col = positions[0][i], positions[1][i]
                        gen_val = gen_img[row, col]
                        ref_val = ref_img[row, col]
                        difference = diff[row, col]
                        print(f"  Position ({row},{col}): Generated={gen_val}, Reference={ref_val}, Diff={difference}")
            
            return False
            
    except Exception as e:
        print(f"❌ Error: {e}")
        return False

def main():
    # Set up paths
    build_dir = Path("/Users/lvxuanyi/Desktop/CSC317/computer-graphics-raster-images/build")
    validation_dir = Path("/Users/lvxuanyi/Desktop/CSC317/computer-graphics-raster-images/data/validation")
    
    # Get list of PPM files in validation directory
    validation_files = list(validation_dir.glob("*.ppm"))
    
    if not validation_files:
        print("❌ No PPM files found in validation directory")
        return
    
    # Set tolerance (can be adjusted)
    base_tolerance = 1  # Allow small differences due to rounding
    demosaic_tolerance = 3  # Optimized tolerance after improvements
    
    print("🔍 PPM Validation Results")
    print("=" * 50)
    
    passed = 0
    failed = 0
    
    for ref_file in sorted(validation_files):
        gen_file = build_dir / ref_file.name
        
        print(f"\n📁 {ref_file.name}")
        print("-" * 30)
        
        if not gen_file.exists():
            print(f"❌ Generated file not found: {gen_file}")
            failed += 1
            continue
        
        # Use higher tolerance for demosaicking
        tolerance = demosaic_tolerance if 'demosaic' in ref_file.name else base_tolerance
            
        if compare_ppm_files(gen_file, ref_file, tolerance):
            passed += 1
        else:
            failed += 1
    
    print("\n" + "=" * 50)
    print(f"📊 SUMMARY: {passed} passed, {failed} failed")
    
    if failed == 0:
        print("🎉 All files validated successfully!")
    else:
        print("⚠️  Some files failed validation. Check the differences above.")

if __name__ == "__main__":
    main()
