using JSON

ge76_molarmass = 75.9214027E-3 # kg/mol
ge_molarmass = 72.64E-3 # kg/mol
ge76_rho = 5.5E-3 # kg/cm3
ge_rho = 5.32E-3 # kg/cm3
ge76_nat_fr = 0.078

function mbb(thalf)
    thalf == "nan" && return ["nan", "nan"]
    NME = [2.89, 6.13]
    return [sqrt(1 / thalf / 2.36e-15 / 1.25^4 / NME[2]^2 * 511e3^2), sqrt(1 / thalf / 2.36e-15 / 1.25^4 / NME[1]^2 * 511e3^2)]
end

j = JSON.parsefile("ge76-history.json")["ge76-history"]

header = ["date", "exposure_kgyr", "bkg_idx", "bkg_idx_err",
          "T0nu_sensitivity", "T0nu_val", "T0nu_val_err_up",
          "T0nu_val_err_low", "T0nu_68_limit", "T0nu_90_limit",
          "mbb_68_limit_up", "mbb_68_limit_low",
          "mbb_90_limit_up", "mbb_90_limit_low",
          "mbb_68_limit_today_up", "mbb_68_limit_today_low",
          "mbb_90_limit_today_up", "mbb_90_limit_today_low",
          "label"]

for (exp, val) in j
    fout = open("$exp.csv", "w")
    println(fout, join(header, ", "))
    id = 1
    while haskey(val, string(id))
        data = val[string(id)]
        @info "$exp: $(data["doi"])"

        exposure = "nan"
        tot_exposure = "nan"
        if haskey(data, "ge76-exposure-kgyr")
            exposure = data["ge76-exposure-kgyr"]

        elseif haskey(data, "ge76-exposure-molyr")
            exposure = data["ge76-exposure-molyr"]*ge76_molarmass

        elseif haskey(data, "ge-exposure-kgyr")
            exposure = data["ge-exposure-kgyr"]*ge76_nat_fr

        elseif haskey(data, "volume-cm3")
            if data["volume-cm3"] isa Array
                exposure = cumsum((data["volume-cm3"] .* data["livetime-yr"])*ge76_rho*ge76_nat_fr)[end]
                tot_exposure = cumsum((data["volume-cm3"] .* data["livetime-yr"])*ge_rho)[end]
            else
                exposure = data["volume-cm3"]*data["livetime-yr"]*ge76_rho*ge76_nat_fr
                tot_exposure = data["volume-cm3"]*data["livetime-yr"]*ge_rho
            end
        end

        BI = "nan"
        if haskey(data, "BI-ctskeVkgyr")
            BI = data["BI-ctskeVkgyr"]

        elseif haskey(data, "BI-ctskeVmolyr")
            BI = data["BI-ctskeVmolyr"]/ge_molarmass

        elseif haskey(data, "volume-cm3")
            if data["volume-cm3"] isa Array
                BI = cumsum((data["BI-ctskeVh"]*24*365) .* data["livetime-yr"])[end]/tot_exposure
            else
                BI = data["BI-ctskeVh"]*24*365*data["livetime-yr"]/tot_exposure
            end
        end

        label = ""
        if data["enr-germanium"] && data["underground"]
            label = "uenr"
        elseif !data["enr-germanium"] && !data["underground"]
            label = "anat"
        elseif !data["enr-germanium"] && data["underground"]
            label = "unat"
        end

        T0nu_val_err_up = "nan"
        T0nu_val_err_low = "nan"
        if haskey(data, "T0nu-68-CI-yr")
            T0nu_val_err_up = data["T0nu-68-CI-yr"][2] - data["T0nu-best-yr"]
            T0nu_val_err_low = data["T0nu-best-yr"] - data["T0nu-68-CI-yr"][1]
        elseif haskey(data, "T0nu-99-CI-yr")
            T0nu_val_err_up = data["T0nu-99-CI-yr"][2] - data["T0nu-best-yr"]
            T0nu_val_err_low = data["T0nu-best-yr"] - data["T0nu-99-CI-yr"][1]
        end

        line = [
            data["published"],
            exposure,
            BI,
            get(data, "BI-err-ctskeVkgyr", "nan"),
            get(data, "sensitivity", "nan"),
            get(data, "T0nu-best-yr", "nan"),
            T0nu_val_err_up,
            T0nu_val_err_low,
            get(data, "T0nu-68-limit-yr", "nan"),
            get(data, "T0nu-90-limit-yr", "nan"),
            get(data, "mbb-68-limit-eV", ["nan", "nan"])[1],
            get(data, "mbb-68-limit-eV", ["nan", "nan"])[2],
            get(data, "mbb-90-limit-eV", ["nan", "nan"])[1],
            get(data, "mbb-90-limit-eV", ["nan", "nan"])[2],
            mbb(get(data, "T0nu-68-limit-yr", "nan"))[1],
            mbb(get(data, "T0nu-68-limit-yr", "nan"))[2],
            mbb(get(data, "T0nu-90-limit-yr", "nan"))[1],
            mbb(get(data, "T0nu-90-limit-yr", "nan"))[2],
            label
        ]
        println(fout, join(line, ", "))
        id += 1
    end
    close(fout)
end
