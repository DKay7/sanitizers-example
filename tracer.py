import subprocess
import sys


def process_trace_line(line: str):
    bb_id = line.split(";")[0]

    module_and_offset = line.split(";")[1].split()[0]
    module, offset = module_and_offset.replace("(", "").replace(")", "").split("+")

    return bb_id, module, offset

def parse_trace_info(trace_filename: str):
    with open(trace_filename, "r") as file:
        lines = file.readlines()
    
    bbs = set()
    modules = set()
    trace = list()
    for line in lines:
        bb_id, module_name, offset = process_trace_line(line)
        bbs.add(bb_id)
        trace.append((bb_id, offset, module_name))
        modules.add(module_name)

    return bbs, modules, trace

def write_parsed_trace_info(result_filename: str, bbs: set, modules: set, trace: list):

    with open(result_filename, "w") as file:
        file.write(f"#DRCOV VERSION: 2\n#DRCOV FLAVOR: isp\n\n#Module Table: count {len(modules)}\n#Columns: id, module_name\n")
                
        for idx, module in enumerate(modules):
            file.write(f"{idx}, {module}\n")
        
        file.write(f"\n#BB Table: {len(bbs)} bbs\n#Columns: id, offset, module_name\n")

        for bb_id, offset, module_name in trace:
            file.write(f"{bb_id}, {offset}, {module_name}\n")
        
def run_program(prog_name):
    subprocess.run(prog_name)

    bbs, modules, trace = parse_trace_info("isp-compiler.trace")
    
    out_filename = prog_name + ".trace"
    write_parsed_trace_info(out_filename , bbs, modules, trace)
    print(f"Trace has been saved to {out_filename}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 tracer.py <path to instrumented binary>")
        print("Note: make sure to delete `isp-compiler.trace` file to avoid mixing trace info")
        exit(0)

    run_program(sys.argv[1])
